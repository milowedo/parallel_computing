import os

from pyspark.ml.linalg import Vectors
from pyspark.ml.regression import LinearRegression
from pyspark.sql import SparkSession

if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'
    spark = SparkSession.builder \
        .master("local") \
        .appName("regression") \
        .getOrCreate()

    train = spark.createDataFrame([
        (5.0, Vectors.dense([1.0])),
        (6.0, Vectors.dense([2.0])),
        (7.0, Vectors.dense([3.0]))],
        ["label", "features"])

    test = spark.createDataFrame([
        (14.0, Vectors.dense([10.0])),
        (24.0, Vectors.dense([20.0])),
        (34.0, Vectors.dense([30.0]))],
        ["label", "features"])

    train.show()
    lr = LinearRegression(maxIter=10, regParam=0.01)
    model = lr.fit(train)
    model.transform(test).show()

    spark.stop()
