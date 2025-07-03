# Open-source Cache Dataset

The open-source cache dataset were compiled from multiple sources, including Microsoft, CloudPhysics, Tencent, Alibaba, Twitter, Meta production systems. 
We provide both plain text and [oracleGeneral](#oraclegeneral-format-structure) format. 

## Table of Contents
- [Dataset Description](#dataset-description)
- [Data Organization](#data-organization)
- [Object Cache Traces](#object-cache-traces)  
- [Block Cache Traces](#block-cache-traces)
- [How to Use the Dataset](#how-to-use-the-dataset)
- [Tutorial: Using Cache Dataset on AWS](#tutorial-using-cache-dataset-on-aws)
- [License](#license)
- [Contact](#contact)

## Use Cases

You can use the dataset to perform different tasks, including but not limited to:
* **Evaluation**: Testing your caching systems (Memcached, Redis, database bufferpool)
* **Analysis**: Gaining insights about production systems and observing access patterns (diurnal, weekly)
* **Research**: Designing and evaluating new distributed systems and databases

## Data Storage & Access

The datasets are stored in AWS S3. You have two options:
1. **Local Download**: Download traces to your local cluster
2. **Cloud Computing**: Launch EC2 instances for computation

**Recommendation**: Since the dataset is large, we recommend provisioning a cluster to run computations. You can use [mountpoint](https://github.com/awslabs/mountpoint-s3) to mount the bucket on each node and [distComp](https://github.com/1a1a11a/distComp) to launch computation jobs. 


## Dataset Description

### Dataset Summary

| Cache Type | Dataset | Year | Time span (days) | # Trace | # Request (million) | Request (TB) | # Object (million) | Object (TB) | Source | Txt format | OracleGeneral format |
|:---:|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **Key-value** | [MetaKV](#meta-key-value-cache-traces) | 2022 | 1 | 5 | 1,644 | 958 | 82 | 76 |  |  |  |
| | [Twitter](#twitter-twemcache-request-traces) | 2020 | 7 | 54 | 195,441 | 106 | 10,650 | 6 |  |  |  |
| **Object** | [MetaCDN](#meta-cdn-request-traces) | 2023 | 7 | 3 | 231 | 8,800 | 76 | 1,563 |  |  |  |
| | [Wikimedia CDN](#wikimedia-cdn-request-traces) | 2019 | 7 | 3 | 2,863 | 200 | 56 | 13 |  |  |  |
| | [Tencent Photo](#tencent-photo-cdn-request-traces) | 2018 | 8 | 2 | 5,650 | 141 | 1,038 | 24 |  |  |  |
| | [IBM Docker](#ibm-docker-dataset) | 2018 | - | - | - | - | - | - |  |  |  |
| **Block** | [Google](#google-synthetic-io-traces) | 2024 | - | - | - | - | - | - |  |  |  |
| | [MetaStorage](#meta-tectonic-cache-traces) | 2021 | 5 | 3000 | - | - | - | - |  |  |  |
| | [Tencent CBS](#tencent-cloud-ebs-traces) | 2020 | 8 | 4030 | 33,690 | 1091 | 551 | 66 |  |  |  |
| | [Alibaba Block](#alibaba-cloud-ebs-traces) | 2020 | 30 | 652 | 19,676 | 664 | 1702 | 117 |  |  |  |
| | [CloudPhysics](#cloudphysics-traces) | 2015 | 7 | 106 | 2,114 | 82 | 492 | 22 |  |  |  |
| | [Microsoft Cambridge](#msr-cambridge-traces) | 2007 | 7 | 13 | 410 | 10 | 74 | 3 |  |  |  |

**Note**: A more detailed description of each dataset can be found in the source link and the sections below.

## Data Organization

The cache dataset is organized into three main categories based on cache type:

### 1. Key-value Cache Traces
These traces capture requests to in-memory key-value stores like Memcached and Redis. They include:
- **Twitter Twemcache**: 54 clusters, 7 days, 195M+ requests
- **Meta CacheLib**: 2 datasets from different time periods with varying cluster sizes

### 2. Object Cache Traces  
These traces represent content delivery networks (CDNs) and object storage systems:
- **Meta CDN**: Edge cache clusters with multi-TB workloads
- **Wikimedia CDN**: Multiple datasets spanning different years
- **Tencent Photo CDN**: Photo storage system with cache hit information
- **IBM Docker Registry**: Container image storage and retrieval patterns

### 3. Block Cache Traces
These traces capture block-level I/O operations from storage systems:
- **Google Synthetic I/O**: Validated synthetic traces representing Google's storage clusters
- **Enterprise Storage**: MSR Cambridge traces from production data centers
- **Cloud Storage**: Alibaba and Tencent cloud block storage traces
- **Virtualized Storage**: CloudPhysics traces from VMware environments

### Dataset Formats

We provide both plain text format that is human readable and `oracleGeneral` format that is suitable for using with [libCacheSim](https://libcachesim.com) platform. 

#### OracleGeneral Format Structure

```c
struct {
    uint32_t timestamp;
    uint64_t obj_id;
    uint32_t obj_size;
    int64_t next_access_vtime;  // -1 if no next access
}
```

#### Compression

All datasets are compressed with [zstd](https://github.com/facebook/zstd). You can use `zstd -d` to decompress the data. 

**Note**: libCacheSim can directly work with compressed data, so no decompression is needed if you use libCacheSim to run simulations. 


# Key-value Cache Traces

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


# Object Cache Traces

## Wikimedia CDN Request Traces

This dataset contains traces from Wikimedia CDN infrastructure. It includes datasets collected at different times (2008, 2016, and 2019) with different formats. We omit the 2008 version as it doesn't have object size information and is incomplete.

The original release can be found at [here](https://wikitech.wikimedia.org/wiki/Data_Platform/Data_Lake/Traffic/Caching)

### 2019 

This dataset is a restricted snapshot of the `wmf.webrequest` table. It consists of 42 compressed files covering 21 days of data: 21 files contain upload (image) request data (`cache-u`), and 21 contain text pageview request data (`cache-t`). Each file covered an exactly 24-hour period. <!-- For open-sourced traces, 21 traces are contactnated as one file respectively and object id is salted. -->

#### Upload Data Format

- `relative_unix`: Seconds since dataset start
- `hashed_path_query`: Salted hash of request path and query
- `image_type`: Image type: `jpeg`, `png`, `gif`, `svg+xml`, `x-icon`, `tiff`, `webp`, `vnd.djvu`, `x-xcf`
- `response_size`: Response size in bytes
- `time_firstbyte`: Time to first byte in seconds (float)

#### Text Data Format

- `relative_unix`: Seconds since dataset start
- `hashed_host_path_query`: Salted hash of host, path, and query
- `response_size`: Response size in bytes
- `time_firstbyte`: Time to first byte in seconds

### 2016

These are request traces collected from Wikimedia’s upload cache infrastructure. The traces span two consecutive weeks and were captured from a single front-end cache node (`cp4006`) located in the ulsfo data center. Each node has (roughly) 96 GB of DRAM for memory caching and (roughly) 720 GB of SSD for disk caching. The trace was filtered to include only user traffic with 200 OK responses, and hashed URLs to preserve privacy. The effective sampling corresponds to requests routed through a single cache. The data includes anonymized object identifiers, response sizes, content types, and latency metrics, and has been used for performance evaluation of cache replacement policies.


#### Trace Format

- `hashed_host_and_path`: Salted hash of request host and path
- `uri_query`: Full requested URL with query parameters (plaintext)
- `content_type`: MIME type from HTTP Content-Type header
- `response_size`: Bytes sent until first byte of response
- `X_Cache`: CDN caching metadata and cache hierarchy

### Download Links
- **Plain text (2019)**: https://analytics.wikimedia.org/published/datasets/caching/2019/
- **Plain text (2016)**: Currently not accessible
- **OracleGeneral format**: 

---

## Tencent Photo CDN Request Traces

This is a photo CDN request dataset collected in 2016 over a span of 9 consecutive days at the sampling ratio of 1/100.
It captured real-world production workloads from Tencent’s large-scale photo storage system, QQPhoto, including cache hit results under an LRU policy with a total cache size of (roughly) 5TB.

QQPhoto supports separate photo upload and download channels and two-tier cache is employed for photo download, including outside caches and data center caches. The details of the trace can be found in [Demystifying Cache Policies for Photo Stores at Scale: A Tencent Case Study](https://dl.acm.org/doi/10.1145/3205289.3205299)

### Trace Format

- `timestamp`: Request time in `YYYYMMDDHHMMSS` format
- `photo_id`: Hexadecimal checksum of requested photo
- `image_format`: `0` = jpg, `5` = webp
- `size_category`: Image size tier: `l`, `a`, `o`, `m`, `c`, `b` (increasing sizes)
- `return_size`: Bytes returned for requested image
- `cache_hit`: `1` = cache hit, `0` = cache miss
- `terminal_type`: Device type: `PHONE` or `PC`
- `response_time`: Response time in milliseconds (`0` = <1ms)

#### Size Category Reference
- `l`: 33,136 bytes
- `a`: 3,263,749 bytes
- `o`: 4,925,317 bytes
- `m`: 6,043,467 bytes
- `c`: 6,050,183 bytes
- `b`: 8,387,821 bytes

### Download Links
- **Plain text**: https://iotta.snia.org/traces/parallel/27476
- **OracleGeneral format**: 

---

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

### Trace Format
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

## IBM Docker Dataset

This dataset contains Docker Registry traces from IBM's infrastructure, captured over 75 days (June 20 - September 2, 2017) across 7 availability zones. The traces analyze production workload patterns for container image storage and retrieval, processing 208 GB of raw data into 22.4 GB of processed traces with 38M+ requests and 180TB+ data transfer.

The details of the trace can be found in [Improving Docker Registry Design based on Production Workload Analysis](https://www.usenix.org/conference/fast18/presentation/anwar).

### Trace Format

- `host`: Anonymized registry server
- `http.request.duration`: Response time in seconds
- `http.request.method`: HTTP method (GET, PUT, HEAD, PATCH, POST)
- `http.request.remoteaddr`: Anonymized client IP
- `http.request.uri`: Anonymized requested URL
- `http.request.useragent`: Docker client version
- `http.response.status`: HTTP response code
- `http.response.written`: Data received/sent in bytes
- `id`: Unique request identifier
- `timestamp`: Request arrival time (UTC)

### Download Links
- **Plain text**: http://iotta.snia.org/tracetypes/6
- **OracleGeneral format**: 


---



# Block Cache Traces

## Google Synthetic I/O Traces

This dataset contains synthetically generated I/O traces that represent Google's storage cluster workloads. The traces are created using advanced synthesis techniques to produce realistic I/O patterns that closely match actual production disk behaviors observed in Google's data centers.

The synthetic traces are generated for multiple disk categories across different storage clusters, with detailed validation performed over multiple days. The primary dataset showcases a week-long trace from February 24 to March 2, 2023, focusing on the largest disk category in one storage cluster. The synthesis methodology has been validated across various disk types and storage clusters, demonstrating consistent accuracy. Samping rate at 1/10,000.

The details of the trace can be found in [Thesios: Synthesizing Accurate Counterfactual I/O Traces from I/O Samples](https://dl.acm.org/doi/10.1145/3620666.3651337).

### Trace Format

- `filename`: Local filename
- `file_offset`: File offset
- `application`: Application owner of the file
- `c_time`: File creation time
- `io_zone`: Warm or cold
- `redundancy_type`: Replicated or erasure-coded
- `op_type`: Read or write
- `service_class`: Request's priority: latency-sensitive, throughput-oriented, or other
- `from_flash_cache`: Whether the request is from flash cache
- `cache_hit`: Whether the request is served by server's buffer cache
- `request_io_size_bytes`: Size of the request
- `disk_io_size_bytes`: Size of the disk operation
- `response_io_size_bytes`: Size of the response
- `start_time`: Request's arrival time at the server
- `disk_time`: Disk read time (for cache-miss read)
- `latency`: Latency of the operation (from arrival time to response time at the server)
- `simulated_disk_start_time`: Start time of disk read (for cache-miss read)
- `simulated_latency`: Latency of the operation (adjusted by the trace reorganizer)

### Download Links
* **Plain text**: https://console.cloud.google.com/storage/browser/thesios-io-traces
* **OracleGeneral format**: 


## MSR Cambridge Traces
This is a block-level I/O trace collected in February 2007. It captures activity from 36 volumes (comprising 179 disks) across 13 enterprise servers in a production data center over the course of one week, starting at 5 PM GMT on February 22, 2007.

Each server was configured with a RAID-1 boot volume using two internal disks, and one or more RAID-5 data volumes backed by co-located, rack-mounted DAS. All servers ran Windows Server 2003 SP2, with data stored on NTFS volumes and accessed via protocols such as CIFS and HTTP. I/O requests were recorded using the Event Tracing for Windows (ETW) tool. We note this dataset was collected to study power-saving strategies in enterprise storage systems.

Details of the trace can be found in [Write Off-Loading: Practical Power Management for Enterprise Storage](https://www.usenix.org/legacy/event/fast08/tech/full_papers/narayanan/narayanan.pdf).

### Trace format

- `Timestamp`: The time the I/O was issued, recorded in “Windows filetime” format.
- `Hostname`: The hostname of the server, which should match the hostname in the trace file name.
- `DiskNumber`: The logical disk number, which should match the disk number in the trace file name.
- `Type`: The type of I/O operation, either “Read” or “Write”.
- `Offset`: The starting offset of the I/O request in bytes from the beginning of the logical disk.
- `Size`: The transfer size of the I/O request in bytes.
- `ResponseTime`: The time taken by the I/O to complete, measured in Windows filetime units.

### Download Links
* **Plain text**: https://iotta.snia.org/traces/block-io/388
* **OracleGeneral format**: 

---

## CloudPhysics Traces

This block-level I/O trace dataset was collected from 106 virtual disks on VMware ESXi hypervisors in production environments for one week. The traces were recorded with VMware’s vscsiStats. Local sampling was used when full trace uploads and corresponding storage analysis weren't needed.

The traced VMs run Linux or Windows, with disk sizes from 8 GB to 34 TB (median 90 GB), memory up to 64 GB (median 6 GB), and up to 32 vCPUs (median 2).

The details of the trace can be found in [Efficient MRC Construction with SHARDS](https://www.usenix.org/system/files/conference/fast15/fast15-paper-waldspurger.pdf).

### Trace Format

- `timestamp`: The time the I/O request was issued, in microseconds
- `lbn`: The starting Logical Block Number (LBA) of the I/O request, in sectors
- `len`: The size of the I/O request, in sectors
- `cmd`: The SCSI command code indicating the access type (e.g., read or write)
- `ver`: A version field used to distinguish between VSCSI1 and VSCSI2 formats

### Download Links
* **Plain text**:
* **OracleGeneral format**: 

---

## Tencent Cloud EBS Traces

This dataset consists of 216 I/O trace files collected from a production cloud block storage (CBS) system over a 10-day period (October 1–10, 2018). The traces originate from a single warehouse (also known as a failure domain), covering I/O requests from 5,584 cloud virtual volumes (CVVs). These requests were ultimately redirected to a storage cluster comprising 40 storage nodes (i.e., disks).

These traces were well-suited for per-volume analysis, i.e., studying access patterns of individual CVVs by grouping requests by VolumeID. More details on the dataset can be found in [OSCA: An Online-Model Based Cache Allocation Scheme in Cloud Block Storage Systems](https://www.usenix.org/conference/atc20/presentation/zhang-yu).

### Trace Format

- `Timestamp`: Time the I/O request was issued, given in Unix time (seconds since Jan 1, 1970)
- `Offset`: Starting offset of the I/O request, in sectors (1 sector = 512 bytes) from the beginning of the logical volume for the original released traces (converted to bytes in our open-sourced traces)
- `Size`: Transfer size of the I/O request, in sectors
- `IOType`: Type of operation — 0 for Read, 1 for Write
- `VolumeID`: Anonymized ID of the cloud virtual volume (CVV)

### Download Links
* **Plain text**: https://iotta.snia.org/traces/parallel/27917
* **OracleGeneral format**: 

---

## Alibaba Cloud EBS Traces

The dataset was collected from a production cluster of Alibaba Cloud's Elastic Block Storage (EBS) service, which provides virtual disk storage. A total of 1,000 virtual disks were randomly sampled from the cluster. All I/O activities to these disks were recorded for the entire month of January 2020. 

The selected disks are Ultra Disk products, a cost-effective tier in Alibaba Cloud’s block storage offerings. Ultra Disks are backed by a distributed storage system that ensures high reliability but with relatively lower random I/O performance compared to Standard SSD or Enhanced SSD products. Typical applications of Ultra Disks include OS hosting, web servers, and big data workloads.

### Trace Format

#### io_traces.csv
- `device_id`: ID of the virtual disk, remapped to the range 0 ∼ 999
- `opcode`: Operation type — R for Read, W for Write
- `offset`: Byte offset of the operation from the beginning of the disk
- `length`: Length of the I/O operation in bytes
- `timestamp`: Time the operation was received by the server, in microseconds since the Unix epoch

#### device_size.csv
- `device_id`: ID of the virtual disk
- `capacity`: Capacity of the virtual disk in bytes

### Download Links
* **Plain text**: https://github.com/alibaba/block-traces
* **OracleGeneral format**: 

---

## Meta Tectonic Cache Traces
Those are traces captured for 5 consecutive days from a Meta's [block storage](https://engineering.fb.com/2021/06/21/data-infrastructure/tectonic-file-system) cluster consisting of 3000 hosts at the sampling ratio of 1/4000. 
Each host uses (roughly) 10 GB of DRAM and 380 GB of SSD for caching. 
The open-source traces were merged from multiple hosts and the effective sampling ratio of is around 1.


### Trace Format

- `op_time`: The time of the request
- `block_id`: The requested block ID
- `block_id_size`: The size of the requested block (in MB), this field is always 40
- `io_size`: The requested size, note that requests often do not ask for the full block
- `io_offset`: The start offset in the block
- `user_name`: Anonymized username (represent different use cases)
- `user_namespace`: Anonymized namespace, can ignore, only one value
- `op_name`: Operation name, can be one of `getChunkData.NotInitialized`, `getChunkData.Permanent`, `putChunk.NotInitialized`, and `putChunk.Permanent`
- `op_count`: The number of requests in the same second
- `host_name`: Anonymized host name that serves the request
- `rs_shard_id`: Reed-solomon shard ID


### Download Links
* **Plain text**: 
* **OracleGeneral format**: 

---



# How to Use the Dataset

Due to the large size of these datasets, we recommend using larger servers for computation, such as AWS EC2 VMs.

## Tutorial: Using Cache Dataset on AWS

### Prerequisites
- AWS Account with appropriate permissions
- Basic familiarity with AWS S3 and EC2 services
- Understanding of cache systems and trace analysis

### Step 1: Setting Up Your AWS Environment

#### Option 1: Using EC2 for Computation
1. Launch an EC2 instance with sufficient storage and memory
2. Recommended instance types: `m5.xlarge` or larger for analysis workloads
3. Ensure your instance has adequate EBS storage or use instance store

#### Option 2: Using AWS S3 Direct Access
1. Configure AWS CLI with your credentials
2. Use S3 transfer acceleration for faster downloads
3. Consider using AWS Batch or ECS for large-scale processing

### Step 2: Accessing the Dataset

The cache datasets are hosted in AWS S3. You can access them in several ways:

#### Using AWS CLI
```bash
# List available datasets
aws s3 ls s3://cache-dataset-bucket/

# Download specific traces
aws s3 cp s3://cache-dataset-bucket/twitter/ ./twitter-traces/ --recursive

# Use sync for partial downloads
aws s3 sync s3://cache-dataset-bucket/meta-kv/ ./meta-kv-traces/
```

#### Using Mountpoint-S3 (Recommended for Large Datasets)
Install and use [mountpoint-s3](https://github.com/awslabs/mountpoint-s3) to mount the S3 bucket:

```bash
# Install mountpoint-s3
curl -L https://s3.amazonaws.com/mountpoint-s3-release/latest/x86_64/mount-s3.deb -o mount-s3.deb
sudo dpkg -i mount-s3.deb

# Mount the bucket
mkdir /mnt/cache-data
mount-s3 cache-dataset-bucket /mnt/cache-data

# Access files directly
ls /mnt/cache-data/
```

### Step 3: Working with Compressed Data

All datasets are compressed with zstd. Here are common approaches:

#### Decompressing Files
```bash
# Decompress a single file
zstd -d twitter_cluster01.zst

# Decompress multiple files
find . -name "*.zst" -exec zstd -d {} \;
```

#### Working with Compressed Data Directly
If using libCacheSim, you can work directly with compressed files:

```bash
# Install libCacheSim
git clone https://github.com/1a1a11a/libCacheSim.git
cd libCacheSim && mkdir build && cd build
cmake .. && make -j

# Run cache simulation on compressed data
./cachesim twitter_cluster01.zst oracleGeneral LRU 1000000
```

### Step 4: Data Analysis Examples

#### Basic Statistics with Python
```python
import pandas as pd
import matplotlib.pyplot as plt

# Read a trace file (assuming CSV format)
df = pd.read_csv('trace_file.csv')

# Basic statistics
print(f"Total requests: {len(df)}")
print(f"Unique objects: {df['object_id'].nunique()}")
print(f"Average object size: {df['object_size'].mean()}")

# Plot request rate over time
df['timestamp'] = pd.to_datetime(df['timestamp'])
hourly_requests = df.set_index('timestamp').resample('H').size()
hourly_requests.plot(title='Request Rate Over Time')
plt.show()
```

#### Cache Hit Ratio Analysis
```python
# Analyze cache hit patterns
hit_ratio = df['cache_hit'].mean()
print(f"Overall hit ratio: {hit_ratio:.2%}")

# Hit ratio by object size
size_bins = pd.cut(df['object_size'], bins=10)
hit_by_size = df.groupby(size_bins)['cache_hit'].mean()
hit_by_size.plot(kind='bar', title='Hit Ratio by Object Size')
plt.show()
```

### Step 5: Running Cache Simulations

#### Using libCacheSim
```bash
# LRU simulation with 100MB cache
./cachesim trace.oracleGeneral oracleGeneral LRU 100000000

# Compare multiple algorithms
for algo in LRU LFU FIFO; do
    echo "Testing $algo"
    ./cachesim trace.oracleGeneral oracleGeneral $algo 100000000
done
```

#### Using Custom Scripts
Create your own analysis scripts for specific research questions:

```python
# Example: Analyzing temporal locality
def analyze_reuse_distance(trace_df):
    reuse_distances = []
    last_access = {}
    
    for idx, row in trace_df.iterrows():
        obj_id = row['object_id']
        if obj_id in last_access:
            reuse_distances.append(idx - last_access[obj_id])
        last_access[obj_id] = idx
    
    return reuse_distances

distances = analyze_reuse_distance(df)
plt.hist(distances, bins=50, log=True)
plt.xlabel('Reuse Distance')
plt.ylabel('Frequency')
plt.title('Reuse Distance Distribution')
plt.show()
```

### Step 6: Best Practices for Large-Scale Analysis

#### Memory Management
- Use chunked processing for large files
- Consider using Dask for out-of-core computation
- Implement sampling strategies for initial exploration

#### Storage Optimization
- Use S3 Intelligent Tiering for cost optimization
- Consider data partitioning strategies
- Implement checkpointing for long-running analyses

#### Parallel Processing
- Use AWS Batch for embarrassingly parallel workloads
- Consider EMR for Spark-based processing
- Utilize multiple EC2 instances with shared storage

### Cost Optimization Tips

1. **Use Spot Instances**: Significant cost savings for batch workloads
2. **Right-size Your Instances**: Match compute requirements to instance types
3. **S3 Transfer Optimization**: Use S3 Transfer Acceleration and VPC endpoints
4. **Data Lifecycle**: Move processed results to cheaper storage classes

### Support and Resources

- Join our [Google Group](https://groups.google.com/g/cache-trace) for questions
- Check the [libCacheSim documentation](https://libcachesim.com) for simulation tools
- Review AWS documentation for [S3](https://docs.aws.amazon.com/s3/) and [EC2](https://docs.aws.amazon.com/ec2/) best practices

---

## Reading the Dataset

*This section provides detailed instructions for reading and processing the datasets in different formats.*

## Plotting and Analyzing the Dataset

*This section includes examples and best practices for data analysis and visualization.*

## Running Cache Simulations

*This section provides guides for running cache simulations using libCacheSim and other tools.*

### Example Jupyter Notebook on AWS SageMaker

*Step-by-step tutorial for getting started with the datasets using AWS SageMaker.*

---

## AWS Open Data Registry

This dataset is available through the AWS Open Data Sponsorship Program and is listed in the [Registry of Open Data on AWS](https://registry.opendata.aws/). The dataset meets all AWS Open Data requirements including:

- **Open License**: Released under CC BY 4.0 license allowing free use with attribution
- **Public Access**: Freely accessible via AWS S3 without authentication
- **Documentation**: Comprehensive documentation with usage examples and tutorials
- **Community Value**: Supports research and development in cache systems and distributed computing
- **Data Quality**: Well-structured, validated production traces from leading technology companies

### Registry Entry
The complete AWS Open Data registry entry can be found in `aws-open-data.yaml` in this repository.

### Data Location
- **S3 Bucket**: `s3://cache-dataset-bucket/`
- **Region**: `us-east-1`
- **Access**: Public read access, no AWS credentials required

### Usage on AWS
This dataset is optimized for use on AWS infrastructure:
- Direct S3 access for cost-effective storage
- Integration with EC2, EMR, and SageMaker for analysis
- Support for AWS Batch and Lambda for serverless processing
- Compatible with mountpoint-s3 for file system access

---

## Note on Curated Datasets

Some open-source datasets are not included in this release. These datasets often do not provide a clear description of how the data were collected.

---

## Changelog

- **2024/12/01**: First version

---

## Contact

If you have any questions, please join the [Google Group](https://groups.google.com/g/cache-trace) or [Slack].

---

## License

This work is licensed under the **Creative Commons Attribution 4.0 International Public License (CC BY 4.0)**. To obtain a copy of this license, see LICENSE-CC-BY-4.0.txt in the archive, visit CreativeCommons.org or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

### Terms:

**Attribution** — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

**No additional restrictions** — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

---

## Acknowledgements

The original traces were collected by contributors from multiple institutes, including **Meta, Twitter, CloudPhysics, Microsoft, Wikimedia, Alibaba, Tencent**, and several others.

This collection and converted traces are open-sourced by **Juncheng Yang** from School of Engineering and Applied Science at Harvard University.

The storage of this dataset is sponsored by **AWS** under open data agreement.

---

## Papers Using This Dataset

If you would like your paper to be featured here, please send a PR.



