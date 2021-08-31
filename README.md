# LightspeedKV
Bypasses Linux Networking stack through XDP to implement an ultra-low latency key-value store

# Motivation
This project was born out of frustration with the latencies of popular remote memory-caches - they all suffer from bottlenecks induced by the Linux network stack. I explored using eBPFs to build a cache that safely serves simple key-value cache requests in-kernel through the eXpress Data Path (XDP). By avoiding user-kernel boundary crossings, this approach is performant under high-throughput workloads. In the future, I'd like to build a performant proxy using eBPFs enabling the distribution of data among multiple LightspeedKV nodes and explore potential for offloading to SmartNICs.

# Architecture
<img width="239" alt="Screen Shot 2021-08-31 at 1 03 48 PM" src="https://user-images.githubusercontent.com/26693584/131568477-f3bb0b9f-0ec9-4bc3-974a-def185cb5f0c.png">

