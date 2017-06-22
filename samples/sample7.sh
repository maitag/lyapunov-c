cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
(x+a)^sin(x-a)/sin(x)
0-i*240
0
i*100
400
300
-1.3
2.1
-2.2
3.3
1
10
100
EOF
