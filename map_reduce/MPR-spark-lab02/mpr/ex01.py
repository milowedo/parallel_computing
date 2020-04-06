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

    rdd = sc.textFile('../text').flatMap(lambda line: line.split(' ')).map(lambda w: Row(w))
    df = rdd.toDF(['word'])
    # peek the content of the DataFrame (use just for debug! as it can execute the while pipeline)
    df.show()
    # write as csv
    # df.write.csv('result')

    time.sleep(300)

    spark.stop()
