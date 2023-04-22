# `keygrabber`

```
$ sudo ./keygrabber /dev/input/by-id/usb-Keychron_Keychron_C1-event-kbd
MYPASSWORD IN OTHER APP
```

# `memspy`/`readchar`

```
$ ./readchar
19541
0x7fffffffbdf7
X
Y
Z
```

```
$ ./memspy /proc/19541/mem 0x7fffffffbdf7
Z
```
