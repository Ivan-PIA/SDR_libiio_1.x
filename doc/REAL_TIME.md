# Реальный прием:

1. Реализовано:
    - разбитие на потоки считывание блока и обработка блока

```c++
std::vector<comp> read_from_block_real(char *ip)
{	
	std::mutex buffer_mutex;
    size_t rx_sample_sz;
    struct iio_device *rx;
    const struct iio_block *rxblock;
    std::vector<comp> temp;
    std::vector<comp> rx_data;

	std::vector<comp> data_with_pss;
	std::vector<comp> all_buffer;
    // Инициализация устройства
    rx = initialize_device_rx(ip, rx_sample_sz, rxblock, rx, rxstream);

    int block_counter = 0;
    std::vector<std::vector<comp>> buffer_set;  // Для хранения трех буферов

	
	show_log(CONSOLE, "\t*** START RECIVING ***\n");
    
    while (!stop)
    {
        rxblock = iio_stream_get_next_block(rxstream);
        temp = process_rx_block(rxblock, rx_sample_sz);

        // Добавляем текущий буфер данных в общий
        rx_data.insert(rx_data.end(), temp.begin(), temp.end());

        // Сохраняем буфер в буферный набор
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer_set.push_back(temp);
        }

        block_counter++;

        // Если накоплено три буфера, запускаем функцию свертки в отдельном потоке
        if (block_counter == 3)
        {
            // Асинхронный вызов функции свертки
            std::future<std::pair<int, std::vector<comp>>> conv_and_concat_result = std::async(std::launch::async, [&buffer_set]() {
                std::vector<comp> combined_data;
                std::vector<comp> concatenated_buffer;

    
                // Склеиваем три буфера в один для свертки
                for (const auto& buf : buffer_set) {
                    combined_data.insert(combined_data.end(), buf.begin(), buf.end());
                }

                // Выполняем свертку на склеенном буфере
                int convolution_result = convolve(combined_data);

                
                concatenated_buffer.insert(concatenated_buffer.end(), combined_data.begin(), combined_data.end());

                return std::make_pair(convolution_result, concatenated_buffer);
            });
			

            // Ожидаем завершения свертки
            // int result = conv_result.get();
			auto [convolution_result, concatenated_buffer] = conv_and_concat_result.get();

            // Если результат свертки больше 1, останавливаем прием данных
			
            if (convolution_result > 9)
            {
				{
					std::lock_guard<std::mutex> lock(buffer_mutex);
					data_with_pss = flatten(buffer_set);
				}
                stop = true;
				show_log(CONSOLE, "\n\t  *** PSS FOUND ***\n\n");
				all_buffer = concatenated_buffer;
            }

            // Очищаем буферный набор и увеличиваем счетчик блоков
            {
                std::lock_guard<std::mutex> lock(buffer_mutex);
                buffer_set.clear();
            }
            block_counter = 0;
        }
    }
	write_to_file(rx_data);

    return data_with_pss;
}
```

- результат бесконечного прослушивания и одиночной передачи на 1 SDR:
 
 <img src = "photo\real_time.png">

 в накопленных буферах был найден **PSS** и дан сигнал и выводе, в дальнейшем сигнал о отправки буферов на обрезки по PSS.

- Ресурсная сетка для этого сигнала

 <img src = "photo\real.png">