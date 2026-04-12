#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define LED_PIN 12
#define BUTTON1_PIN 5
#define BUTTON2_PIN 6
#define WIFI_SSID "Ana luiza"
#define WIFI_PASS "luiz@3020"
#define BUZZER_PIN 21

char http_response[2048];

void sound_buzzer(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t clock_freq = clock_get_hz(clk_sys);

    uint32_t wrap = clock_freq / frequency - 1;
    pwm_set_wrap(slice_num, wrap);
    pwm_set_gpio_level(BUZZER_PIN, wrap /2);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}
void create_http_response() {
    snprintf(http_response, sizeof(http_response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n"
             "Connection: close\r\n"
             "\r\n"
             "<!DOCTYPE html><html lang=\"pt-br\"><head>"
             "<meta charset=\"UTF-8\"><title>Pomoclock</title>"
             "<style>"
             "body { display: flex; justify-content: center; align-items: center; flex-direction: column; font-family: Arial; background-color: antiquewhite; height: 100vh; margin: 0; }"
             "#startButton, #restButton { padding: 10px 20px; font-size: 16px; cursor: pointer; border-radius: 10px; background-color: #f23737; color: white; border: none; margin: 5px; }"
             "#startButton:disabled, #restButton:disabled { background-color: gray; cursor: default; }"
             "#container { display: flex; justify-content: center; align-items: center; flex-direction: column; background-color: white; padding: 50px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }"
             "</style></head><body>"
             "<div id=\"container\"><h1>POMOCLOCK</h1><h1 id=\"countdown\">25:00</h1>"
             "<div id=\"buttons\"><button id=\"startButton\">Estudar</button><button id=\"restButton\">Descansar</button></div></div>"
             "<script>"
             "let time = 25 * 60; let timerInterval = null; const countdownElement = document.getElementById('countdown');"
             "function updateCountdown() { "
             "  let minutes = Math.floor(time/60); const seconds = time - (minutes*60); "
             "  countdownElement.innerHTML = `${minutes}:${seconds<10?'0'+seconds:seconds}`;"
             "}"
             "function startTimer(duration) {"
             "if(timerInterval) clearInterval(timerInterval); time = duration; updateCountdown();"
             "timerInterval = setInterval(() => {"
             "if(time<=0) {clearInterval(timerInterval); fetch('/timeout');} else {time--; updateCountdown();}"
            " },1000)"
             "}"
             "document.getElementById('startButton').addEventListener('click', () => { startTimer(25 * 60); document.getElementById('startButton').disabled = true; document.getElementById('restButton').disabled = false;});"
             "document.getElementById('restButton').addEventListener('click', () => { startTimer(5 * 60); document.getElementById('restButton').disabled = true; document.getElementById('startButton').disabled = false; });"
             "</script></body></html>");
}


static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }

char *request = (char *)p->payload;
if(strstr(request, "GET /timeout") != NULL) sound_buzzer(2000, 500);
else if(strstr(request, "GET /led/off") != NULL) {
    gpio_put(LED_PIN, 1);
    sleep_ms(1000);
    gpio_put(LED_PIN, 0);
}

create_http_response();
tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);
pbuf_free(p);
return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);
    return ERR_OK;
}

static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if(!pcb) return;
    if(tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) return;
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
}

void monitor_buttons() {
static bool button1_last_state = false;
static bool button2_last_state = false;

bool button1_state = !gpio_get(BUTTON1_PIN);
bool button2_state = !gpio_get(BUTTON2_PIN);


if(button1_state != button1_last_state) {
    button1_last_state = button1_state;
    if(button1_state) {
        snprintf(http_response, sizeof(http_response), "Button 1 Pressed\n");
    } else {
        snprintf(http_response, sizeof(http_response), "Button 1 Released\n");
    }
}

if(button2_state != button2_last_state) {
    button2_last_state = button2_state;
    if(button2_state) {
        snprintf(http_response, sizeof(http_response), "Button 2 Pressed\n");
    } else {
        snprintf(http_response, sizeof(http_response), "Button 2 Released\n");
    }
}
}



int main()
{
    stdio_init_all();
    sleep_ms(2000);

    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("WiFi connection failed\n");
        return -1;
    } else {
        uint8_t *ip_adress = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("Connected to WiFi with IP address: %d.%d.%d.%d\n", ip_adress[0], ip_adress[1], ip_adress[2], ip_adress[3]);
    }
    printf("Wifi conectado\n");

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON1_PIN);
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON1_PIN);

    gpio_init(BUTTON2_PIN);
    gpio_set_dir(BUTTON2_PIN, GPIO_IN);
    gpio_pull_up(BUTTON2_PIN);

    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, true);

    start_http_server();

    while (true) {
     cyw43_arch_poll();
    monitor_buttons();
    sleep_ms(100);
   // sound_buzzer(1000, 100);
    }

    cyw43_arch_deinit();
    return 0;
}
