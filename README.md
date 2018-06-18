# newVector(4-oji užduotis)
 ![alt text](https://github.com/PauliusKu/newVector/blob/developer/WordItOut-word-cloud-3145844.png)

## Programos pagrindinės funkcijos:
 - visos pagrindinės funkcijos sutampa su std::vector funkcijomis;
 
 ## Spartos analizė:

### push_back funkcijos palyginimas (testuota po 100 kartų 10^1 - 101^7):

| Flag| performance increase (%) |
| ------------| ------------- |
| -Od | 51.2743   |
| -O1 | 37.7629  |
| -O2 | 39.9743  |

### std::vector ir new vector palyginimas Studento programoje ():

| Flag| Container      | time (s) (size = 10^6) | performance increase (%) |
| ------------| ------------- | ------------- |------------- |
| -Od | std::vector   | 31.5154  |   |
| -Od | newvector   | 26.9974   |  14.333   |
| -O1 | std::vector   | 5.15318  |    |
| -O1 | newvector   | 5.51525  |  -7.026  |
| -O2 | std::vector   | 5.33299  |    |
| -O2 | newvector   | 5.59541  |  -4.920  |
