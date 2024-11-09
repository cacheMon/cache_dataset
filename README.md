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





## Key-value cache traces
### Twitter Twemcache request traces

Description


Foramt

Original release https://github.com/twitter/cache-trace

Raw dataset download link: 


Citation


### Meta Key-value cache traces



### 






### Object cache 
#### Wikipedia CDN request traces


#### Tencent Photo CDN request traces


#### Meta CDN request traces



### Block cache
#### MSR Cambridge traces


#### CloudPhysics traces



#### Tencent Cloud EBS traces



#### Alibaba Cloud EBS traces


#### Meta Tectonic cache traces


## How to use the dataset
Because the size of these datasets, we recommend you to larger servers to run computation, such as VMs from AWS EC2.  


### 

### Reading the dataset 


### Plotting and analyzing the dataset


### Running cache simulations


### An example jupyter notebook on AWS Sagemaker 


## Changelog


## Contact


## References


## License
This work is licensed under the Creative Commons Attribution 4.0 International Public License (CC BY 4.0). To obtain a copy of this license, see LICENSE-CC-BY-4.0.txt in the archive, visit CreativeCommons.org or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

Under the following terms:

Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.


## Acknowledgements



## Papers that used this dataset



