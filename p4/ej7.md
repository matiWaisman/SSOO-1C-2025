Una secuencia, que no creo que sea la mas simple, puede ser: 0,1,2,0,2,3,0,1

Usando LRU:

| Página pedida | Frames | Páginas a desalojar |
|---------------|---------------------------|---------------------|
| 0             | [<span style="color:red">0</span>, _, _] | 0                   |
| 1             | [0, <span style="color:red">1</span>, _] | 0 1                 |
| 2             | [0, 1, <span style="color:red">2</span>] | 0 1 2               |
| 0             | [<span style="color:green">0</span>, 1, 2] | 1 2 0             |
| 2             | [0, 1, <span style="color:green">2</span>] | 1 0 2             |
| 3             | [0, <span style="color:red">3</span>, 2] | 0 2 3            |
| 0             | [<span style="color:green">0</span>, 3, 2] | 2 3 0            |
| 1             | [0, 3, <span style="color:red">1</span>] | 3 0 1           |

Usando Second Chance:

| Página pedida | Frames | Páginas a desalojar |
|---------------|---------------------------|---------------------|
| 0             | [<span style="color:red">0</span>, _, _] | 0                   |
| 1             | [0, <span style="color:red">1</span>, _] | 0 1                 |
| 2             | [0, 1, <span style="color:red">2</span>] | 0 1 2               |
| 0             | [<span style="color:green">0</span>, 1, 2] | <span style="color:yellow">0</span> 1 2             |
| 2             | [0, 1, <span style="color:green">2</span>] | <span style="color:yellow">0</span> 1 <span style="color:yellow">2</span>             |
| 3             | [0, <span style="color:red">3</span>, 2] | <span style="color:yellow">2</span> 0 3             |
| 0             | [<span style="color:green">0</span>, 3, 2] | <span style="color:yellow">2</span> <span style="color:yellow">0</span> 3             |
| 1             | [0, <span style="color:red">1</span>, 2] | 2 0 1             |


