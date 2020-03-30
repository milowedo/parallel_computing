import os
import time
import re

from pyspark.sql import SparkSession

# $ cat puma_short.log | wc
#  277544 3141529 46336075
# $ cat puma_short.log | grep experiment_stats | wc
#   32446  385855 4233939


if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'

    spark = SparkSession.builder \
        .master("local") \
        .appName("Top requests") \
        .getOrCreate()
    sc = spark.sparkContext

    print(sc.uiWebUrl)

    file = sc.textFile('puma_short.log')
    if __name__ == '__main__':
        filtered = file.filter(lambda line: re.match(r".*/experiments/[a-z0-9]*?/experiment_stats.*", line))
        top10 = filtered.map(lambda line: re.match(r".*experiments/([a-z0-9]*?)/experiment_stats.*", line).group(1)) \
            .map(lambda word: (word, 1)) \
            .reduceByKey(lambda a, b: a + b) \
            .keyBy(lambda a: a[1]).sortByKey(False, None).values().take(10)

    print(top10)


    time.sleep(300)

    spark.stop()
