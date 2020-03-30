import os
import time

from pyspark import RDD
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

    print(sc.uiWebUrl)

    text_file = sc.textFile('text')
    if __name__ == '__main__':
        counts = text_file.flatMap(lambda line: line.split(' ')) \
            .map(lambda word: (word, 1)) \
            .reduceByKey(lambda a, b: a + b).keyBy(lambda a: a[1]).sortByKey(False, None).values()

    counts.saveAsTextFile('output01')

    file = open('results/ex01.txt', 'w')
    file.write(str((counts.take(10))).strip('[]'))
    file.close()

    time.sleep(300)

    spark.stop()
