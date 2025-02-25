# Многолучевый канал

Канал передачи – среда распространения радиоволн между передатчиком и приемником вносит самые большие искажения в сигнал и оказывает наибольшее влияние на передачу данных.

<img src = "photo\mul.png">

Модель канала состоит из аддитивной и мультипликативной помехи

𝐱 – переданный сигнал 

g – канальный коэффициент

n – шум

<img src = "photo\ng.png">

- Аддитивная помеха, это когда итоговый сигнал изначального сигнала x и помехи n
-  Мультипликативная помеха, это когда итоговый сигнал y является результатом перемножения изначального сигнала x и канального коэффициента g

## Мультипликативная помеха

### Разность фаз:

$$2\pi\frac{d_i-d_1}{\lambda}$$


$\lambda$ - длина волны

$d_1$ - кротчайший путь

$d_i$ - длина i-го пути


### Канальный коэффициент многолучевого канала:


$$g = \sum_{i=1}^{n} a_i e^{-j\phi}$$

$L$ – количество лучей

$a_i$ - затухание амплитуды сигнала на i-м пути 

### Задержка (для проверки символьной синхронизации):
После расчета величина 𝜏 должна быть округлена до ближайшего целого значения. В математической модели одним из вариантов реализации задержки в 𝑖 м луче может быть добавление целого количества 𝜏 нулей перед сигнальными отчетами:

$$S_i(k)=0 ,если k <= t_i$$

$$S_i(k)= x_{tx}(k-t_i) ,если k > t_i$$

где 𝑖 = 1,2,…,𝑁–номер луча 𝑘 =1,2,…,𝐿+𝜏 начальном векторе –номер отсчета в сигнал с выхода передатчика
𝑁 – кол-во лучей

$$g = \sum_{i=1}^{N} a_i S_i$$

$a_i$ - ослабление i-го луча


## Затухание сигнала
Причины возникновения α– естественное ослабление сигнала в пространстве.

Ослабление по амплитуде в разах:

$$X\frac{c}{4\pi d f}$$

$X$ - коэффициент ослабления среды распространения

$c$ – скорость света

$d$ – длина луча

$f$ – несущая частота

## Эффект Допплера

$$f_c\frac{v}{c}cos(\theta)$$


$f_c$ - несущая частота

$v $ - скорость абонента

$c$ - скорость света

$\theta$ - угол между направлением на источник и вектором скорости в системе отсчета приемника

## Применение помех

$$y(t) = g(t)* S_m(t) * e^{-j2\pi\Delta ft} + n$$

## Результат
### Характеристики:

защитные нули - 27

кол-во пилот - 12

кол-во лучей - 10

c = 3 * 1e8  - м/с

ширина полосы = 1.92 * 1e6 - Hz

несущая = 2 * 1e9 - Hz

скорость абонента - 100 км/ч

угол – 20
### Частотная характеристика канала

<img src = "photo\ach.png">

Созвездия справа многолучевый канал, слева созвездия после оценки канала и коррекции по пилотам.

<img src = "photo\qa.png">
