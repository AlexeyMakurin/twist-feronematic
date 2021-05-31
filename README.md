# Twist Ferronematic

## Description
This program is designed to simulate the stratification of ferroparticles caused by gravitational and magnetic fields in soft ferronematics.

More details about the problem itself and what a ferronematic is can be found in this [*article*][1].

## Using

Program requires a third-party library [*JSON for Modern C++*][2].

The executable file must be passed a json file of the following form:

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

### Example

[1]:https://iopscience.iop.org/article/10.1088/1742-6596/1389/1/012058
[2]:https://github.com/nlohmann/json/releases/tag/v3.9.1