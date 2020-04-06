import os
import time

from pyspark import Row
from pyspark.sql import SparkSession

if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'
        
    spark = SparkSession.builder \
        .master("local") \
        .appName("Word Count") \
        .getOrCreate()

    sc = spark.sparkContext

    rdd = sc.textFile('../text'). \
        flatMap(lambda line: line.split(' ')) \
        .map(lambda item: item.replace(',', '')) \
        .filter(lambda item: item.strip()) \
        .map(lambda w: Row(w))
    df = rdd.toDF(['word']) \
        .groupBy('word') \
        .count() \
        .sort("count", ascending=False)
    df.show(10)
    # write as csv
    df.write.save('ex01.txt')

    time.sleep(300)

    spark.stop()
