import numpy
import time
import h5py

import matplotlib.pyplot as plt
import scipy
from PIL import Image
from scipy import ndimage
from lib import *
import matplotlib.pyplot

### PLOT SETUP

plt.rcParams['figure.figsize'] = (5.0, 4.0) # set default size of plots
plt.rcParams['image.interpolation'] = 'nearest'
plt.rcParams['image.cmap'] = 'gray'

### PREPARE DATA AND SET PARAMETERS

# load images
X_train, Y_train, X_test, Y_test, classes = load_data()
# number of trials
m_train=X_train.shape[0]
m_test=X_test.shape[0]
# input size (= number of image's pixels)
n_x=X_train.shape[1]*X_train.shape[1]*3
# output size (= a label for cat or non-cat)
n_y=1
# flatten pixel matrix to column array
X_train = X_train.reshape(X_train.shape[0], -1).T
X_test = X_test.reshape(X_test.shape[0], -1).T
# normalise data to values between 0 and 1
X_train = X_train/255.
X_test = X_test/255.
# layers' sizes
layers_dims = [n_x, 20, 7, 5, n_y]
# number of layers
L = len(layers_dims)-1
# number of iterations
num_iterations=2500

# weights, biases, activation functions, derivatives w.r.t. them and costs
parsWb={}
parsZA={}
parsdWdbdA={}
costs=[]

# activation functions
# notice: dAL formula in backward_pass hardcodes choice of "sigmoid" for g3
parsg = {"g1": "relu",
         "g2": "relu",
         "g3": "relu",
         "g4": "sigmoid"}

# activation functions and derivatives
def relu(x):
    return numpy.maximum(0,x)
def reluprime(x):
    x[x>0]=1
    x[x<1]=0
    return x
def sigmoid(x):
    return 1/(1+numpy.exp(-x))
def sigmoidprime(x):
    return sigmoid(x)*(1-sigmoid(x))

# learning rate in gradient descent
learning_rate=0.0075

# randomise seed
numpy.random.seed(1)
#numpy.random.seed(int(time.time()))

### FUNCTIONS
 
# initialise weights and biases
def initialiseWb():
    global parsWb
    for l in range(1,L+1):
        parsWb["W"+str(l)]=numpy.random.randn(layers_dims[l],layers_dims[l-1])/numpy.sqrt(layers_dims[l-1])
        parsWb["b"+str(l)]=numpy.zeros((layers_dims[l],1))
    
# forward pass
def forward_pass():
    global parsZA
    for l in range(1,L+1):
        # get values
        W=parsWb["W"+str(l)]
        b=parsWb["b"+str(l)]
        A=parsZA["A"+str(l-1)]
        # calculate values
        Z=numpy.dot(W,A)+b
        parsZA["Z"+str(l)]=Z
        parsZA["A"+str(l)]=(relu(Z) if parsg["g"+str(l)]=="relu" else sigmoid(Z))

# calculate cost and return it
def calculate_cost(m,Y):
    global parsZA
    AL=parsZA["A"+str(L)]
    return numpy.squeeze(-numpy.sum(Y*numpy.log(AL)+(1-Y)*numpy.log(1-AL))/m)

# backward pass
def backward_pass(m,Y):
    global parsdWdbdA
    AL=parsZA["A"+str(L)]
    dAL=-numpy.divide(Y,AL)+numpy.divide(1-Y,1-AL)
    parsdWdbdA["dA"+str(L)]=dAL
    for l in reversed(range(1,L+1)):
        # get values
        W=parsWb["W"+str(l)]
        A=parsZA["A"+str(l-1)]
        Z=parsZA["Z"+str(l)]
        dA=parsdWdbdA["dA"+str(l)]
        dZ=dA*(reluprime(Z) if parsg["g"+str(l)]=="relu" else sigmoidprime(Z))
        # calculate values
        parsdWdbdA["dW"+str(l)]=1./m*numpy.dot(dZ,A.T)
        parsdWdbdA["db"+str(l)]=1./m*numpy.sum(dZ,axis=1,keepdims=True)
        parsdWdbdA["dA"+str(l-1)]=numpy.dot(W.T,dZ)
        # no need to save dZ

# gradient descent
def gradient_descent():
    global parsWb
    for l in range(1,L+1):
        parsWb["W"+str(l)]=parsWb["W"+str(l)]-learning_rate*parsdWdbdA["dW"+str(l)]
        parsWb["b"+str(l)]=parsWb["b"+str(l)]-learning_rate*parsdWdbdA["db"+str(l)]

# run training or test, depending on input variables
# calls functions above
def run_model(m,X,Y):
    # weights, biases, activation functions, derivatives w.r.t. them and costs
    global parsWb, parsZA, parsdWdbdA, costs
    parsWb={}
    initialiseWb()
    parsZA={"A0": X}
    parsdWdbdA={}
    costs=[]

    # run
    for i in range(num_iterations):
        forward_pass()
        if (i%100==0) or (i==num_iterations-1):
            costs.append(calculate_cost(m,Y))
            print("Cost after iteration "+str(i)+": "+str(costs[-1]))
        backward_pass(m,Y)
        gradient_descent()

    # plot
    matplotlib.pyplot.plot(numpy.squeeze(costs))
    matplotlib.pyplot.ylabel("cost")
    matplotlib.pyplot.xlabel("iterations (per hundreds)")
    matplotlib.pyplot.title("training")
    matplotlib.pyplot.show()

# make predictions
def predict(m,X,Y):
    global parsZA
    parsZA={"A0": X}
    forward_pass()
    Yhat=parsZA["A"+str(L)]

    Yhat[Yhat>0.5]=1
    Yhat[Yhat<0.5]=0
    print("Accuracy: "+str(numpy.sum((Yhat==Y)/m)))

    return Yhat

### MAIN FUNCTIONS
print("\nLearning on training images")
run_model(m_train,X_train,Y_train)

print("\nMake predictions on training images")
Yhat_train=predict(m_train,X_train,Y_train)

print("\nMake predictions on test images")
Yhat_test=predict(m_test,X_test,Y_test)
print("\n")