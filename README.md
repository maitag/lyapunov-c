## lyapunov-c
This little command line tool calculates the [lyapunov](https://en.wikipedia.org/wiki/Aleksandr_Lyapunov)-[indices](https://en.wikipedia.org/wiki/Chaotic_mixing#Lyapunov_exponents) from a function input  
and puts out an image in [plain ppm format](http://netpbm.sourceforge.net/doc/ppm.html#plainppm).  
  
It simmulates behavior of a dynamic system to get fractal  
images like this:  
  
![](samples/sample5.jpg?raw=true)


### Build binary and test
```
make
sh test.sh
```



### Use interactive first

```
./lyapunov
```
to see order of input parameters.  



### Generate images

On linux you can use image converting tools like netpbm
```
aptitude install netpbm
```
and little shell script to pipe output throught ppmtojpeg (or other format convertor):
```
cat <<EOF | ./lyapunov -p | ppmtojpeg > ${0%.*}.jpg
2.5*sin(x+a)^2+2
0-i*200
0
i*200
400
300
-1
1
-1
1
0
10
100
EOF
```



### Todo
- symbolic differentation for equations like ``f(x) = a^g(x)``


