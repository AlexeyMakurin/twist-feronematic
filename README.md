# Twist Ferronematic (TF)

## Description
This program is designed to simulate the stratification of ferroparticles caused by gravitational and magnetic fields in soft ferronematics.

More detail about this problem and the meaning of ferronematic, you can found in this [*article*][1].

## Using

Program requires a third-party library [*JSON for Modern C++*][2].


***The program executable file*** must be passed a json file of the following form:

```
{
    "twist_ferronematics": [
        {"h": 1.0, "alpha": 2.0, "b": 3.0, "sigma": 1.0, "kappa": 0.2, "nodes": 20}
    ]

}
```
- The parameter *h* characterizes the magnetic field in which the twist ferronematic  is located.
- The *alpha*, *b*, *sigma*, *kappa* parameters are responsible for the properties of an individual twist ferronematic (in detail about each parameter in the [*article*][1]).<br/>
- *nodes* - the number of points of the computational grid required for the numerical solution of the equations of equilibrium state. 

The result of the work will be csv file / files in the created directory - *<current_directory>/results/h = 1.00, alpha = 2.00, b = 3.00, sigma = 1.00, kappa = 0.20, notes = 20.csv*.

### Example
As an example, take the input data file *inputs.json*:
```
{
    "twist_ferronematics": [
        {"h": 0.0, "alpha": 4.0, "b": 3.5, "sigma": 1.2, "kappa": 0.5, "nodes": 100},
        {"h": 1.0, "alpha": 4.0, "b": 3.5, "sigma": 1.2, "kappa": 0.5, "nodes": 100},
        {"h": 2.0, "alpha": 4.0, "b": 3.5, "sigma": 1.2, "kappa": 0.5, "nodes": 100}
    ]

}
```

In this case, three simulations of a twist ferronematic will be calculated. Parameters defining TF properties have the same values. i.e. simulation of one TF, but with different values of the magnetic field.

```
<The program executable file> inputs.json
```



[1]:https://iopscience.iop.org/article/10.1088/1742-6596/1389/1/012058
[2]:https://github.com/nlohmann/json/releases/tag/v3.9.1