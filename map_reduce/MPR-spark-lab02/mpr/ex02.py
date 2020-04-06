import os
import random

from pyspark.ml.linalg import Vectors
from pyspark.ml.regression import LinearRegression, time
from pyspark.sql import SparkSession
from sklearn.metrics import mean_squared_error

if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'
    spark = SparkSession.builder \
        .master("local") \
        .appName("regression") \
        .getOrCreate()

    features = []
    labels = []
    for x in range(40):
        features.append(x)
        labels.append(2*x+1)

    merged = list(zip(features, labels))
    random.shuffle(merged)
    features[:], labels[:] = zip(*merged)

    data = [(labels[x], Vectors.dense([features[x]])) for x in range(40)]

    train = spark.createDataFrame(
        data[:30],
        ["label", "features"])

    test = spark.createDataFrame(
        data[30:],
        ["label", "features"])

    # maxIter = 100, regParam = 0.0,  tol = 1e-6, fitIntercept = True
    # poniżej ręcznie dobrane parametry
    maxIter_param = 70
    regParam_param = 0.01
    tol_param = 1e-29
    fitIntercept_param = True
    lr = LinearRegression(maxIter=maxIter_param, regParam=regParam_param, tol=tol_param, fitIntercept=fitIntercept_param)
    # print(lr.explainParams())

    model = lr.fit(train)
    predictions_table = model.transform(test)
    predictions_table.show()

    # split model into labels and predictions
    labels_test = [int(row.label) for row in predictions_table.select('label').collect()]
    predictions_test = [float(row.prediction) for row in predictions_table.select('prediction').collect()]

    # liczę błąd średniokwadratowy
    ms_error = mean_squared_error(labels_test, predictions_test)
    print("The mean squared error for maxIter: ", maxIter_param, " reg: ", regParam_param, " tol: ", tol_param,  " is: ", ms_error)
    print("Coefficient: ", model.coefficients)
    print("Intercept: ", model.intercept)

    # ########################################################################
    # tutaj dodaje losowy błąd w zakresie [-1,1]
    # ########################################################################
    for i in range(30):
        random_number = random.uniform(-1, 1)
        features[i] += random_number

    data = [(labels[x], Vectors.dense([features[x]])) for x in range(40)]

    train_wError = spark.createDataFrame(
        data[:30],
        ["label", "features"])

    lr = LinearRegression(maxIter=maxIter_param, regParam=regParam_param, tol=tol_param, fitIntercept=fitIntercept_param)

    model = lr.fit(train_wError)
    predictions_table = model.transform(test)
    predictions_table.show()

    # split model into labels and predictions
    labels_test = [float(row.label) for row in predictions_table.select('label').collect()]
    predictions_test = [float(row.prediction) for row in predictions_table.select('prediction').collect()]

    # liczę błąd średniokwadratowy
    ms_error = mean_squared_error(labels_test, predictions_test)
    print("The mean squared error for maxIter with added error [-1,1]: ", maxIter_param, " reg: ", regParam_param, " tol: ", tol_param, " is: ",
          ms_error)
    print("Coefficient: ", model.coefficients)
    print("Intercept: ", model.intercept)

    time.sleep(2000)
    spark.stop()
