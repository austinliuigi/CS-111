# A Kernel Seedling
In this lab, we create a kernel module that creates a process that lives at
`/proc/count`. When a user reads this file, e.g. using `cat`, the output to the
terminal is the number of running processes.

## Building
```shell
make
```

## Running
```shell
sudo insmod proc_count.ko
cat /proc/count
```
Result: 141

## Cleaning Up
```shell
sudo rmmod proc_count
make clean
```

## Testing
```python
python -m unittest
```
Result: OK

Report which kernel release version you tested your module on
(hint: use `uname`, check for options with `man uname`).
It should match release numbers as seen on https://www.kernel.org/.

```shell
uname -r -s -v
```
Linux 5.14.8-arch1-1 #1 SMP PREEMPT Sun, 26 Sep 2021 19:36:15 +0000
