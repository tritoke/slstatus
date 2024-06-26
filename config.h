/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */

/* use unicode vertical bar as separator */
#define SEP " │ "
static const struct arg args[] = {
	/* function format          argument */
	/* CPU info */
	{ cpu_perc, "CPU: %s%%", NULL},
	{ cpu_freq, " %sHz", NULL},
	{ run_command, " %s°C" SEP, "sensors k10temp-pci-00c3 -j | jq '.\"k10temp-pci-00c3\".Tctl.temp1_input' | cut -d. -f1" },

	/* RAM info */
	{ ram_used, "RAM: %s" SEP, NULL },
	
	/* Disk usage info */
	{ disk_free, "/: %s" SEP, "/" },

	/* wifi usage */
	{ wifi_essid, "WiFi: (%s)", "wlan0" },
	{ wifi_perc, " %s%%", "wlan0" },
	{ ipv4, " %s" SEP, "wlan0" },

	/* Etherner */
	{ ipv4, "Eth (%s" , "enp43s0" },
	{ ipv4, "|%s)" SEP, "enp36s0" },

	/* volume */
	{ run_command, "VOL: %s", "if [ \"$(pactl get-sink-mute @DEFAULT_SINK@ | grep -o no)\" = \"no\" ]; then pactl get-sink-volume @DEFAULT_SINK@ | grep -Eo '[^ ]+%' | head -1; else echo \"Mute\"; fi" },

	{ run_command, "%s" SEP, "bat=$(timeout 0.5 bluetoothctl info | grep -i battery | rev | cut -d'(' -f1 | rev | cut -d')' -f1); if [ ! \"$bat\" = \"\" ]; then echo \" BTBAT: $bat%\"; fi" },

	/* date */
	{ datetime, "%s ", "%a %d %b %R:%S %Y" },
};
