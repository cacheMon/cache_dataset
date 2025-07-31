datasets_name = ["2007_wiki/",
                 "2008_msr/",
                 "2016_wiki/",
                 "2017_systor/",
                 "2018_tencentPhoto/",
                 "2019_wiki/",
                 "2020_tencentBlock/",
                 "2022_metaKV/",
                 "2023_metaCDN/",
                 "2023_metaStorage/",
                 "2024_google/"]

# ------------------------------------------------------------
# Dump text format links
# ------------------------------------------------------------

special_link = "https://cache-datasets.s3.amazonaws.com/cache_dataset_txt/2020_twr_cdn.zst"

# Example: [google-s3-txt]: https://s3.amazonaws.com/cache-datasets/index.html#cache_dataset_txt/2024_google
for dataset_name in datasets_name:
    tag = f"{dataset_name.split('/')[0].split('_')[1]}-s3-txt"
    print(f"[{tag}]: https://s3.amazonaws.com/cache-datasets/index.html#cache_dataset_txt/{dataset_name}")

print(f"[twitter-s3-txt]: {special_link}")

# ------------------------------------------------------------
# Dump oracleGeneral format links
# ------------------------------------------------------------
datasets_name = ["2007_msr/",
                 "2015_cloudphysics/",
                 "2018_tencentPhoto/",
                 "2019_wiki/",
                 "2020_alibabaBlock/",
                 "2020_tencentBlock/",
                 "2020_twitter/",
                 "2022_metaCDN/",
                 "2022_metaKV/",
                 "2022_metaStorage/"]

# Example: [msr-s3-oracleGeneral]: https://s3.amazonaws.com/cache-datasets/index.html#cache_dataset_oracleGeneral/2007_msr/
for dataset_name in datasets_name:
    tag = f"{dataset_name.split('/')[0].split('_')[1]}-s3-oracleGeneral"
    print(f"[{tag}]: https://s3.amazonaws.com/cache-datasets/index.html#cache_dataset_oracleGeneral/{dataset_name}")