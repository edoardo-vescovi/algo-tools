import scipy.io
import numpy as np
import math
import matplotlib.pyplot as plt
import sklearn.datasets

def load_dataset(flag):
    if(flag=="diagonal"):
        data = scipy.io.loadmat('datasets/data.mat')
        train_X = data['X'].T
        train_Y = data['y'].T
        test_X = data['Xval'].T
        test_Y = data['yval'].T

    if(flag=="spiral"):
    
        np.random.seed(3)
        train_X, train_Y = sklearn.datasets.make_moons(n_samples=300, noise=.2)
        train_X = train_X.T
        train_Y = train_Y.reshape((1, train_Y.shape[0]))

        np.random.seed(4)
        test_X, test_Y = sklearn.datasets.make_moons(n_samples=300, noise=.2)
        test_X = test_X.T
        test_Y = test_Y.reshape((1, test_Y.shape[0]))

    if(flag=="pie"):
        np.random.seed(0)

        train_X=np.random.rand(300,2)-0.5
        train_Y=np.array(list(list(map(label_points,train_X))))
        train_Y=labelToOnehot(train_Y)
        train_X = train_X.T

        test_X=np.random.rand(300,2)-0.5
        test_Y=np.array(list(list(map(label_points,test_X))))
        test_Y=labelToOnehot(test_Y)
        test_X = test_X.T

    return train_X, train_Y, test_X, test_Y

def label_points(x):
    angle=math.atan2(x[1],x[0])
    if(0<angle<2*math.pi/3):
        return 0
    elif(-2*math.pi/3<angle<0):
        return 2
    else:
        return 1

def labelToOnehot(Y):
    num_labels=len(np.unique(Y))
    tmp=np.zeros((num_labels,len(Y))).astype(int)
    for i in range(len(Y)):
        tmp[Y[i],i]=1
    return tmp

def onehotToLabel(Y):
    tmp=np.zeros(Y.shape[1]).astype(int)
    for i in range(Y.shape[1]):
        tmp[i]=list(Y[:,i]).index(1)
    return tmp

def plot_decision_boundary(model, X, y):
    # Set min and max values and give it some padding
    x_min, x_max = X[0, :].min() - 1, X[0, :].max() + 1
    y_min, y_max = X[1, :].min() - 1, X[1, :].max() + 1
    h = 0.01
    # Generate a grid of points with distance h between them
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    # Predict the function value for the whole grid
    Z = model(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    # Plot the contour and training examples
    plt.contourf(xx, yy, Z, cmap=plt.cm.Spectral)
    plt.ylabel('x2')
    plt.xlabel('x1')
    plt.scatter(X[0, :], X[1, :], c=y, cmap=plt.cm.Spectral)
    plt.show()