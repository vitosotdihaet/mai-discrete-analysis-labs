структура данных - множество непересекающихся множеств
где нормально почитать:
- [википедия](https://ru.wikipedia.org/wiki/Система_непересекающихся_множеств)
- [алгоритмика](https://ru.algorithmica.org/cs/set-structures/dsu/)


$S = \cup_{i=0}^k x_i,\ x_i \cap x_j = \emptyset,\ \forall i \ne j$

$\forall x_i: \exists \Gamma_i \in x_i$, где $\Gamma_i$ - представитель множества $x_i$

$\mathrm{Makeset}(x) \rightarrow \{x\}, \Gamma = x$

$\mathrm{Union}(\Gamma_i, \Gamma_j) \rightarrow x_i \cup x_j, \Gamma = \overline x$

$\mathrm{Find}(x) \rightarrow \Gamma_i: x \in x_i$

$\mathrm{Parent}(x) = x \Rightarrow x = \Gamma_i \in x_i$

[AVL tree visualization](https://www.cs.usfca.edu/~galles/visualization/AVLtree.html)