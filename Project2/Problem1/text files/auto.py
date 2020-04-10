import os
import sys
import subprocess

#1K
f = open("1k_out(1).txt", "a")
args = "./thread 5000 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 1000 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 500 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 100 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 50 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 10 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 5 1000 1k_items.txt 1k".split()
subprocess.call(args, stdout=f, shell=False)

#10K
f = open("10k_out(1).txt", "a")
args = "./thread 5000 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 1000 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 500 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 100 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 50 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 10 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 5 10000 10k_items.txt 10k".split()
subprocess.call(args, stdout=f, shell=False)
f.close()

#100K
f = open("100k_out(1).txt", "a")
args = "./thread 5000 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 1000 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 500 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 100 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 50 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 10 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 5 100000 100k_items.txt 100k".split()
subprocess.call(args, stdout=f, shell=False)
f.close()

#1M

f = open("1m_out(1).txt", "a")
args = "./thread 5000 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 1000 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 500 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 100 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 50 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 10 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)

args = "./thread 5 1000000 1m_items.txt 1M".split()
subprocess.call(args, stdout=f, shell=False)
f.close()