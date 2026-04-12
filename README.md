## Pomoclock

### Objetivo do projeto
O sistema embarcado proposto no projeto se trata de um alarme para o famoso método de estudo “Pomodoro”, consistindo em 25 minutos voltados ao estudo e 5 minutos para descansar.

### Instruções de instalação e execução
Sugere-se o uso da extensão Raspberry Pico no VSCode.
Antes de tudo, deve-se alterar no código a senha e o nome da rede Wifi para a conexão.
Compile e execute pelo cabo USB, abra o Serial Monitor do VSCode e veja o endereço em que abrirá a resposta HTTP do projeto.
Copie o endereço e abra. Por exemplo: http://192.168.11.11/
Lá você poderá utilizar o timer pomodoro, escolhendo entre tempo de estudo e de descanso.

### Dependências/Bibliotecas utilizadas
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_pwm
    hardware_adc

### Ferramentas utilizadas:
- VSCode
- BitDogLab

