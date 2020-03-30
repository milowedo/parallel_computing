from pyspark.context import xrange
from random import random
from pyspark.sql import SparkSession
import os

if os.name == 'nt':
    print('detected windows, setting HADOOP_HOME')
    os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'


spark = SparkSession.builder \
    .master("spark://192.168.1.2:7077") \
    .appName("Estimate PI") \
    .getOrCreate()
sc = spark.sparkContext
NUM_SAMPLES = 100000


def inside(p):
    x, y = random(), random()
    return x*x + y*y < 1


count = sc.parallelize(xrange(0, NUM_SAMPLES)) \
             .filter(inside).count()
print("Pi is roughly %f" % (4.0 * count / NUM_SAMPLES))
