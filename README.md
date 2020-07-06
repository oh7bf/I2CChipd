# I2Chipd 
Read chips with I2C interface on Raspberry Pi.
Chips working with some issues: TMP102.

Install *libsystemd-dev* and enable I2C with *raspi-config*.

Compile code with *make*.

Test the code with

```bash
./i2chipd 2> /dev/null
```

The standard error is used for systemd-journald logging.

To create class documentation run *doxygen* without options on command line.

