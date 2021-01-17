import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np


## read data----------------------

data = pd.read_csv("weather.csv", header=None, names=["MinTemp", "MaxTemp", "Rainfall", "Evaporation", "Sunshine", "WindSpeed9am", "WindSpeed3pm", "WindSpeedAvg", "Humidity9am", "Humidity3pm", "HumidityAvg", "Pressure9am", "Pressure3pm", "PressureAvg", "Cloud9am", "Cloud3pm", "CloudAvg", "Temp9am", "Temp3pm", "TempAvg"
])

data = data.iloc[1:,:]
data = data.drop(["WindSpeed9am", "WindSpeed3pm", "Humidity9am", "Humidity3pm", "Temp9am", "Temp3pm", "Cloud9am", "Cloud3pm", "Pressure9am", "Pressure3pm", "MinTemp", "MaxTemp", "Rainfall", "Evaporation", "WindSpeedAvg", "PressureAvg", "TempAvg"], axis=1)

data.insert(0, "Ones", 1)



## Separate data into x and y

X = data.drop(["Sunshine"], axis=1).apply(pd.to_numeric).div([1, 100, 100])
y = data.iloc[:, 1:2].apply(pd.to_numeric).div(13)


print(X)
print("\n", y)


X = np.matrix(X.values)
y = np.matrix(y.values)
theta = np.matrix(np.array([0,0,0]))

## computing the cost function

def computeCost(X, y, theta):
    z = np.power(((X*theta.T)-y),2)
    
    return np.sum(z)/(2*len(X))



## gradient descent function

def gradientDescent(X, y, theta, alpha, iters):
    temp = np.matrix(np.zeros(theta.shape))
    parameters = int(theta.ravel().shape[1])
    cost = np.zeros(iters)
    
    for i in range(iters):
        error = (X * theta.T) - y
        
        for j in range(parameters):
            term = np.multiply(error, X[:, j])
            temp[0, j] = temp[0, j] - ( ( alpha/len(X) ) * np.sum(term))
        
        theta = temp
        
        cost[i] = computeCost(X, y, theta)
        
    return theta, cost




# now tweek alpha and iterations values

# plt.plot(X["HumidityAvg"].to_numpy(), y["Sunshine"].to_numpy(), 'ro')


alpha = 0.01
iters = 11200

g, cost = gradientDescent(X, y, theta, alpha, iters)


fig, ax = plt.subplots(figsize=(8,5))
ax.plot(np.arange(iters), cost, "r")

print("G: ", g)
print("cost: ", cost[-1])
print("********************************************************")
