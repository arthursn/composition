# Conversion of compositions between mass and mole bases

The following equations are used for converting between mass and molar bases:

$$
x_i = \frac{\frac{w_i}{M_i}}{\sum \frac{x_j}{M_j}}
$$

$$
w_i = \frac{x_i \cdot M_i}{\sum x_j \cdot M_j}
$$

It can be proven that:

$$
\sum x_j \cdot M_j = \frac{1}{\sum \frac{w_j}{M_j}} := M_{avg}
$$

, where $M_{avg}$ is, by definition, the average molar mass of the system.

Then, the first two equations can be written as:

$$
x_i = \frac{w_i \cdot M_{avg}}{M_i}
$$

$$
w_i = \frac{x_i \cdot M_i}{M_{avg}}
$$

Suppose we have a system with $n + 1$ elements. We know beforehand the mole fractions of $k$ elements, and the mass fractions of $n-k$ elements. The mole and mass fractions of the major element ($x_0$ and $w_0$, respectively) are unknown.

By definition we have:

$$
\sum_{i=0}^{n} x_i = 1
$$

$$
\sum_{i=0}^{n} w_i = 1
$$

Since the only known variables are $x_i$ for $i = 1, \dots, k$ and $w_i$ for $i = k+1, \dots, n$,  the equations above can be rewritten as:

$$
x_0 + \sum_{i=1}^{k} x_i + \sum_{i=k+1}^{n} \frac{w_i \cdot M_{avg}}{M_i} = 1
$$

$$
w_0 + \sum_{i=1}^{k} \frac{x_i \cdot M_i}{M_{avg}} + \sum_{i=k+1}^{n} w_i = \frac{x_0 \cdot M_0}{M_{avg}} + \sum_{i=1}^{k} \frac{x_i \cdot M_i}{M_{avg}} + \sum_{i=k+1}^{n} w_i = 1
$$

They can be rearranged:

$$
x_0 + \left( \sum_{i=k+1}^{n} \frac{w_i}{M_i} \right) M_{avg} = 1 - \sum_{i=1}^{k} x_i
$$

$$
M_0 \cdot x_0 + \left( \sum_{i=k+1}^{n} w_i - 1 \right) M_{avg}= -\sum_{i=1}^{k} x_i \cdot M_i
$$

Notice the only unknown variables now are $M_{avg}$ and $x_0$. $M_{avg}$ and $x_0$ can be determined by solving this system of equations, which can be written in the matrix form as:

$$
\begin{bmatrix}
    1 & \sum_{i=k+1}^{n} \frac{w_i}{M_i} \\ 
    M_0 & \sum_{i=k+1}^{n} w_i - 1
\end{bmatrix}
\begin{bmatrix}
    x_0 \\
    M_{avg}
\end{bmatrix}
=
\begin{bmatrix}
    1 - \sum_{i=1}^{k} x_i \\
    -\sum_{i=1}^{k} x_i \cdot M_i
\end{bmatrix}
$$

$$
\begin{bmatrix}
    0 & \left( \sum_{i=k+1}^{n} \frac{w_i}{M_i} \right) M_0 - \sum_{i=k+1}^{n} w_i + 1 \\ 
    M_0 & \sum_{i=k+1}^{n} w_i - 1
\end{bmatrix}
\begin{bmatrix}
    x_0 \\
    M_{avg}
\end{bmatrix}
=
\begin{bmatrix}
    \left( 1 - \sum_{i=1}^{k} x_i \right) M_0 + \sum_{i=1}^{k} x_i \cdot M_i \\
    -\sum_{i=1}^{k} x_i \cdot M_i
\end{bmatrix}
$$

Therefore:

$$
M_{avg} = \frac{\left( 1 - \sum_{i=1}^{k} x_i \right) M_0 + \sum_{i=1}^{k} x_i \cdot M_i}{\left( \sum_{i=k+1}^{n} \frac{w_i}{M_i} \right) M_0 - \sum_{i=k+1}^{n} w_i + 1}
$$

or, more elegantly:

$$
M_{avg} = \frac{M_0 - \sum_{i=1}^{k} \left( M_0 - M_i \right) x_i }
{1 + \sum_{i=k+1}^{n} \left( \frac{M_0}{M_i} - 1\right) w_i}
$$

and:

$$
x_0 = 1 - \sum_{i=1}^{k} x_i - \left( \sum_{i=k+1}^{n} \frac{w_i}{M_i} \right) M_{avg}
$$

With $M_{avg}$ and $x_0$ the conversion between mass and mole bases of any element of the system can be easily done.