# Прием LTE сигнала

- Требовалось записать LTE сигнал и найти корреляцию по 3-ем PSS

1. Принятый сигнал:

- ширина полосы = 2 МГц
- частота дискритизации 2.1 МГц (взята минимально возможная для libiio v1.0, если ставить меньше выдается ошибка: *Error -22 writing to channel "sampling_frequency" value may not be supported.*)
- Несущая частота = 2.02 ГГц (через приложение была найдена ближайшая базовая станция и взяты ее праметры для DL)

<img src = "photo/Screenshot from 2024-11-16 23-11-06.png">

2. Делаем корреляцию по 3 PSS (Root index : 25,29,34) 
  
Генерация PSS  (3GPP TS 36.211 version 13.7.1 Release 13, стр. 127)

 <img src = "photo/corr.jpg">

Выводим корреляцию видим что корреляции практически нет. А на ресурсной сетке мы видели сигнал похожий на PSS, далее за ним следует SSS и канал PBCH. По идеи мы должны увидеть только на одном графике выраженные пики, т.к. автокорреляция pss приближена к нулю.


<img src = "photo/Screenshot from 2024-11-18 15-27-49.png">

3. PSS находится в 6 ресурсных блоках, это значит что сигнал занимает 128 поднесущих, у LTE расстояние между поднесущими 15 КГц, 
 15000 * 128 = 1920000 Гц. Т.к. библиотека v1.0 не дает нам поставить такое значение перейдем на версию ниже, v0.23 и примем сигнал с частотой дискритизации 1.92 МГц




4. Принятый сигнал:

<img src = "photo\Screenshot from 2024-11-17 00-47-18.png">

5. Повторяем 2 пункт:

<img src = "photo\Screenshot from 2024-11-18 18-46-19.png">

- Видим отчетливые корреляционные пики  на PSS c индексом 1.

- пытался сделать частную коррекцию по PSS и по CP, сщещение находит но все равно не то.

<img src = "photo/Screenshot from 2024-11-22 13-50-24.png">


### Частотная синхронизация LTE сигнала

- в прошлый раз не получилось сделать частотную синхронизацию, возможная причина слишком большая разность генераторов на базовой станции и Adalm-Pluto.

- Запустим SRSRAN_4G на Adalm-Pluto:

1. Перепрошить PLuto для использования **timestamp** [Инструкция по установке](https://sibsutis-rush.yonote.ru/share/93c85288-45ca-4532-8eab-4079899a5e1c/doc/adalm-pluto-sdr-timestamp-obnovlenie-firmware-ustanovka-soapysdr-plagina-wx9DGvGkQK) (Автор: Р.В. Ахпашев)

2. Изменить конфигурацию в enb.conf на

```
device_name = soapy
device_args = driver=plutosdr,usb_direct=1,timestamp_every=1920,loopback=0
time_adv_nsamples = 40
```

3. Запуск

```
sudo srsepc/src/srsepc 
sudo srsue/src/srsue --log.all_level=info --log.phy_lib_level=none --log.filename=stdout ue.conf

sudo srsenb/src/srsenb --log.all_level=info --log.phy_lib_level=none --log.filename=stdout enb.conf

```

4. Записали:

- Сигнал во временной области:

<img src = "photo/Screenshot from 2024-12-06 16-35-53.png">

- Корреляция по PSS (мы установили pci = 1 значит это PSS с root index = 29)

<img src = "photo/Screenshot from 2024-12-06 15-58-39.png">


- Ресурсная сетка сигнала (сверху) и сигнал обрезанный по PSS (снизу)

<img src = "photo/Screenshot from 2024-12-06 15-52-13.png">

видим частотное смещение и не однородность сигнала

- Сделаем частотнуя синхронизацию по PSS

<img src = "photo/Screenshot from 2024-12-06 15-52-24.png">

найденое смещение CFO = 2772.6 Hz