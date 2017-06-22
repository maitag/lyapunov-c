cat <<EOF | ../lyapunov -p |ppmtojpeg -q 95 > ${0%.*}.jpg
2.5*sin(x+a)^2*cos(a-x)^3
0-i*110
0
i*100
400
300
-2.3
0.7
-0.7
2.3
1.1
10
100
EOF
