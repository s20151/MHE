set term png
set output "result.png"
plot 'result_1.txt' u 1:2 w lines, 'result_2.txt' u 1:2 w lines, 'result_3.txt' u 1:2 w lines, 
set term png
set output "result.png"
plot 'result_100.txt' u 1:2 w lines, 'result_1000.txt' u 1:2 w lines, 'result_10000.txt' u 1:2 w lines, 
set term png
set output "result.png"
plot 'result_10.txt' u 1:2 w lines, 'result_100.txt' u 1:2 w lines, 'result_1000.txt' u 1:2 w lines, 'result_10000.txt' u 1:2 w lines, 
set term png
set output "result.png"
plot 'result_0.001.txt' u 1:2 w lines, 'result_0.01.txt' u 1:2 w lines, 'result_0.1.txt' u 1:2 w lines, 'result_0.txt' u 1:2 w lines, 'result_1.txt' u 1:2 w lines, 
