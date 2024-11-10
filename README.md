# Open-source Cache Dataset

The open-source cache dataset were compiled from multiple sources, including Microsoft, CloudPhysics, Tencent, Alibaba, Twitter, Meta production systems. 
We provide both plain text and [oracleGeneral](https://github.com/1a1a11a/libCacheSim?tab=readme-ov-file#open-source-cache-traces) format. 

You can use the dataset to perform different tasks, including but not limited to 
* evaluating your caching systems, e.g., Memcached, Redis, database bufferpool
* analysing the dataset to gain insights about how production systems work, and observing different access patterns, such as diurnal or weekly. 
* designing and evaluating new distributed systems and databases. 

The datasets are stored in AWS S3. You can either download the traces to your local cluster, or launch some EC2 instances to perform computation. Since the dataset is large, we recommend provisioning a cluster to run the computation. You can use [mountpoint](https://github.com/awslabs/mountpoint-s3) to mount the bucket on each node and [distComp](https://github.com/1a1a11a/distComp) to launch computation jobs. 


## Dataset description
### Dataset summary
| Dataset 	| Year 	|  	| Cache type 	| Time span (days) 	| # Trace 	| # Request (million) 	| Request  (TB) 	| # Object (million) 	| Object  (TB) 	| Source 	| Txt  format 	| OracleGeneral  format 	|
|---	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|:---:	|
| Microsoft Cambridge 	| 2007 	|  	| block 	| 7 	| 13 	| 410 	| 10 	| 74 	| 3 	|  	|  	|  	|
| FIU 	| 2008-2011 	|  	| block 	| 9-28 	| 9 	| 514 	| 1.7 	| 20 	| 0.057 	|  	|  	|  	|
| CloudPhysics 	| 2015 	|  	| block 	| 7 	| 106 	| 2,114 	| 82 	| 492 	| 22 	|  	|  	|  	|
| Systor 	| 2017 	|  	| block 	| 26 	| 6 	| 3,694 	| 88 	| 421 	| 15 	|  	|  	|  	|
| Tencent Photo 	| 2018 	|  	| object 	| 8 	| 2 	| 5,650 	| 141 	| 1,038 	| 24 	|  	|  	|  	|
| Wikimedia CDN 	| 2019 	|  	| object 	| 7 	| 3 	| 2,863 	| 200 	| 56 	| 13 	|  	|  	|  	|
| Tencent CBS 	| 2020 	|  	| block 	| 8 	| 4030 	| 33,690 	| 1091 	| 551 	| 66 	|  	|  	|  	|
| Alibaba Block 	| 2020 	|  	| block 	| 30 	| 652 	| 19,676 	| 664 	| 1702 	| 117 	|  	|  	|  	|
| Twitter 	| 2020 	|  	| key-value 	| 7 	| 54 	| 195,441 	| 106 	| 10,650 	| 6 	|  	|  	|  	|
| MetaKV 	| 2022 	|  	| key-value 	| 1 	| 5 	| 1,644 	| 958 	| 82 	| 76 	|  	|  	|  	|
| MetaCDN 	| 2023 	|  	| object 	| 7 	| 3 	| 231 	| 8,800 	| 76 	| 1,563 	|  	|  	|  	|


A more detailed description of each dataset can be found in the source link and the sections below. 

### Dataset format 
We provide both plain text format that is human readable and `oracleGeneral` format that is suitable for using with [libCacheSim](https://libcachesim.com) platform. 

The oracleGeneral has the following format. 

```
struct {
    uint32_t timestamp;
    uint64_t obj_id;
    uint32_t obj_size;
    int64_t next_access_vtime;  // -1 if no next access
}
```

All datasets are compressed with [zstd](https://github.com/facebook/zstd), you can use `zstd -d` to decompress the data. Note that libCacheSim can directly work with compressed data, so no decompression is needed if you use libCacheSim to run simulations. 





# Key-value cache traces
## Twitter Twemcache request traces
This dataset continas the traces from Twitter's in-memory key-value caching (Twemcache/Pelikan) clusters. 
The traces were collected from 54 clusters in Mar 2020. The traces are one-week-long. 
Note that these cache clusters are first-level caches, so the data popularity of this dataset is highly skewed. 


The details of the trace can be found in 
[Juncheng Yang, Yao Yue, Rashmi Vinayak, A large scale analysis of hundreds of in-memory cache clusters at Twitter. 14th USENIX Symposium on Operating Systems Design and Implementation (OSDI 20), 2020](https://www.usenix.org/conference/osdi20/presentation/yang).

### Trace Format
The original traces are plain text structured as comma-separated columns. Each row represents one request in the following format.

* `timestamp`: the time when the cache receives the request, in sec
* `anonymized` key: the original key with anonymization
* `key size`: the size of key in bytes
* `value size`: the size of value in bytes, could be 0 if it is a cache miss
* `client id`: the anonymized clients (frontend service) who sends the request
* `operation`: one of get/gets/set/add/replace/cas/append/prepend/delete/incr/decr
* `TTL`: the time-to-live (TTL) of the object set by the client, it is 0 when the request is not a write request.

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---



## Meta Key-value cache traces
This dataset contains traces from Meta [Cachelib](https://cachelib.org). 

It has two datasets collected at different time and has different formats. The original release can be found at [here](https://cachelib.org/docs/Cache_Library_User_Guides/Cachebench_FB_HW_eval).

### 202206
Those are traces captured for 5 consecutive days from a Meta's key-value cache cluster consisting of 500 hosts
Each host uses (roughly) 42 GB of DRAM and 930 GB of SSD for caching. 
The open-source traces were merged from multiple hosts and the effective sampling ratio of is around 1/100.
 

* `key`: anonymized requested object ID
* `op`: operation, `GET` or `SET`,
* `size`: the size of the object, could be 0 if it is a cache miss
* `op_count`: number of operations in the current second
* `key_size`: size of the object ID


### 202401
Those are traces captured for 5 consecutive days from a Meta's key-value cache cluster consisting of 8000 hosts
Each host uses (roughly) 42 GB of DRAM and 930 GB of SSD for caching.The open-source traces were merged from multiple hosts and the effective sampling ratio of is around 1/125.

* `op_time`: the time of the request 
* `key`: anonymized requested object ID
* `key_size`: size of the object ID
* `op`: operation, `GET`, `GET_LEASE`, `SET`, `DELETE`
* `op_count`: number of operations in the current second
* `size`: the size of the object, could be 0 if it is a cache miss
* `cache_hits`: the number of cache hits
* `ttl`: time-to-live in seconds
* `usecase`: identifies the tenant, i.e., application using distributed key-value cache
* `sub_usecase`: further categorize the different traffics from the same usecase, but may be imcomplete or inaccurate


### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---

### 






# Object cache 
## WikiMedia CDN request traces


## Tencent Photo CDN request traces


## Meta CDN request traces
This is a CDN request dataset collected in March 2023. 
These traces were captured from Meta's 3 selected CDN cache clusters (named nha, prn, eag) respectively for 7 days. 

Each cluster consists of 1000's of hosts. Each host uses (roughly) 40 GB of DRAM and 1.8TB of SSD for caching. 
Traffic factor and scaled cache sizes are:
* nha: 1/6.37, DRAM 6006 MB, NVM 272314 MB
* prn: 1/4.58, DRAM 8357 MB, NVM 375956 MB
* eag: 1/13.4, DRAM 2857 MB, NVM 129619 MB

I believe these CDN cache clusters are the edge clusters (rather than FNA clusters) given their sizes and compulsory miss ratios. 
Meta CDN uses a two-layer hierarchy, where the first layer is [FNA cluster](https://partners.facebook.com/network/landing_page) (they are also called MNA, Meta Network Appliance). FNA clusters are small and deployed inside ISP networks. Cache misses from the FNA cluster go to a larger edge cluster deployed in an IXP data center. 

### Trace format
* `timestamp`: the timestamp of the request
* `cacheKey`: anonymized object ID
* `OpType`: unknown, but it seems only contains value 1
* `objectSize`: the size of the object in byte
* `responseSize`: the HTTP response size 
* `responseHeaderSize`: the HTTP response header size,
* `rangeStart`: the start offset of a range request, -1 if it is not a range request
* `rangeEnd` the end offset of a range request, -1 if it is not a range request
* `TTL`: time-to-live in seconds
* `SamplingRate`: trace collection sampling ratio, should be ignored because sampled traces from many hosts are mixed
* `cache_hit`: value 1 indicates this request is a cache hit, 0 indicates a cache miss
* `item_value`: unknown, value is either 0 or 1
* `RequestHandler`: unknown 
* `cdn_content_type_id`: annoymized content type id, it is either int or -
* `vip_type`: unknown 
* 

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---



# Block cache
## MSR Cambridge traces


### Trace format

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---

## CloudPhysics traces



### Trace format

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---

## Tencent Cloud EBS traces



### Trace format

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---

## Alibaba Cloud EBS traces



### Trace format

### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---

## Meta Tectonic cache traces
Those are traces captured for 5 consecutive days from a Meta's [block storage](https://engineering.fb.com/2021/06/21/data-infrastructure/tectonic-file-system) cluster consisting of 3000 hosts at the sampling ratio of 1/4000. 
Each host uses (roughly) 10 GB of DRAM and 380 GB of SSD for caching. 
The open-source traces were merged from multiple hosts and the effective sampling ratio of is around 1.


### Trace format
* `op_time`: the time of the request
* `block_id`: the requested block ID 
* `block_id_size`: the size of the requested block (in MB), this field is always 40
* `io_size`: the requested size, note that requests often do not ask for the full block  
* `io_offset`: the start offset in the block 
* `user_name`: aonymized username (represent different use cases) 
* `user_namespace`: aonymized namespace, can ignore, only one value 
* `op_name`:  operation name, can be one of `getChunkData.NotInitialized`, `getChunkData.Permanent`, `putChunk.NotInitialized`, and `putChunk.Permanent`
* `op_count`: the number of requests in the same second 
* `host_name`: aonymized host name that serves the request 
* `rs_shard_id`: reed-solomon shard ID


### Download link
* **Plain text**: 
* **OracleGeneral format**: 

---



# How to use the dataset
Because the size of these datasets, we recommend you to larger servers to run computation, such as VMs from AWS EC2.  


## Google synthetic I/O traces 



## Baleen dataset 



## IBM docker dataset 




## Reading the dataset 


## Plotting and analyzing the dataset


## Running cache simulations

















### An example jupyter notebook on AWS Sagemaker 

## Note on the curated datasets
Note that some open-source datasets are not included in this release. 
These datasets often do not provide a clear description of how the data were collected.  


## Changelog
* **2024/12/01** first version

## Contact
If you have any question, please join the [Google Group](https://groups.google.com/g/cache-trace) or [Slack]. 


## License
This work is licensed under the Creative Commons Attribution 4.0 International Public License (CC BY 4.0). To obtain a copy of this license, see LICENSE-CC-BY-4.0.txt in the archive, visit CreativeCommons.org or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

Under the following terms:

Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.


## Acknowledgements
The original traces were collected by contributors from multiple institutes, including Meta, Twitter, CloudPhysics, Microsoft, WikiMedia, Alibaba, Tencent, and several others. 
This collection and converted traces are open-sourced by Juncheng Yang from School of Engineering and Applied Science at Harvard University. 
The storage of this dataset is sponsored by AWS under open data agreement. 


## Papers that used this dataset
If you would like your paper to be featured here, please send a PR. 



