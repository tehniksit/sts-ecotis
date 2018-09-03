//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "main.h"
//==============================================================================
// Функция возвращает счётчик времени
//==============================================================================

clock_time_t clock_time(void)
{
  return timerCounter;
}
//==============================================================================


void pub_toner_sup_bk(void)
{
	//reading word from eeprom
	toner_bk_count = eeprom_read_word(&cnt_bk_load);	
	char str[30];
	sprintf(str, "%d", toner_bk_count);
	umqtt_publish(&mqtt, "/System1/bk_count", (uint8_t *) str, strlen(str));
	toner_bk_count++;
	//writing word in eeprom
	eeprom_write_word (&cnt_bk_load, toner_bk_count);
	
}

void pub_toner_sup_c(void)
{
	//reading word from eeprom
	toner_c_count = eeprom_read_word(&cnt_c_load);
	char str[30];
	sprintf(str, "%d", toner_c_count);
	umqtt_publish(&mqtt, "/System1/c_count", (uint8_t *) str, strlen(str));
	toner_c_count++;
	//writing word in eeprom
	eeprom_write_word (&cnt_c_load, toner_c_count);
	
}

void pub_toner_sup_m(void)
{
	//reading word from eeprom
	toner_m_count = eeprom_read_word(&cnt_m_load);
	char str[30];
	sprintf(str, "%d", toner_m_count);
	umqtt_publish(&mqtt, "/System1/m_count", (uint8_t *) str, strlen(str));
	toner_m_count++;
	//writing word in eeprom
	eeprom_write_word (&cnt_m_load, toner_m_count);
}

void pub_toner_sup_y(void)
{
	//reading word from eeprom
	toner_y_count = eeprom_read_word(&cnt_y_load);
	char str[30];
	sprintf(str, "%d", toner_y_count);
	umqtt_publish(&mqtt, "/System1/y_count", (uint8_t *) str, strlen(str));
	toner_y_count++;
	//writing word in eeprom
	eeprom_write_word (&cnt_y_load, toner_y_count);
	
}
void show_state_lcd(void)
{
	if (timer_expired(&lcd_state_timer))
	{
		timer_reset(&lcd_state_timer);
		setpos(0,0);
		str_lcd("     EcoTis     ");
		
		
	}
}
void show_ip_lcd(void)
{
	if (timer_expired(&lcd_ip_timer))
	{
		timer_reset(&lcd_ip_timer);
		char str[16];
		uip_gethostaddr(_ip_addr);
		
		if (_ip_addr[0] == 0)
		{
			sprintf(str, "IP: %d.%d.%d.%d     ", _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
		}
		else
		{
			sprintf(str, "IP:%d.%d.%d.%d ", _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
		}
		
		
		
		
		setpos(0,0);
		str_lcd(str);
		
	}
}

//==============================================================================
// Обработчик входящих MQTT-сообщений
//==============================================================================

static void handle_message(struct umqtt_connection *conn, char *topic, char *data)
{
	uint8_t toner_bk = umqtt_isTopicMatched(MQTT_TOPIC_TONER_SUPPLY1, topic);
	if (toner_bk)
	{
		
		load_bk_toner();
		
	}
	uint8_t toner_c = umqtt_isTopicMatched(MQTT_TOPIC_TONER_SUPPLY2, topic);
	if (toner_c)
	{
		load_c_toner();
		
	}
	uint8_t toner_m = umqtt_isTopicMatched(MQTT_TOPIC_TONER_SUPPLY3, topic);
	if (toner_m)
	{
		load_m_toner();
		
		
	}
	uint8_t toner_y = umqtt_isTopicMatched(MQTT_TOPIC_TONER_SUPPLY4, topic);
	if (toner_y)
	{
		load_y_toner();
		
		
	}
}

//==============================================================================


//==============================================================================
// Обработчик прерывания от таймера (период = 1 мс). Используется всеми программными таймерами
//==============================================================================
void msTick_Handler(void) 
{
  timerCounter++;
}
//==============================================================================


//==============================================================================
// Процедура инициализирует 1мс таймера (для работы программных таймеров)
//==============================================================================
void clock_init(void)
{
  tmr2_init(CLOCK_SECOND, msTick_Handler);
  timerCounter = 0;
  tmr2_start();
}
//==============================================================================


//==============================================================================
// Процедура обработчик успешного получения параметров IP от DHCP-сервера
//==============================================================================
void dhcpc_configured(const struct dhcpc_state *s)
{
  uip_sethostaddr(&s->ipaddr);
  uip_setnetmask(&s->netmask);
  uip_setdraddr(&s->default_router);
}
//==============================================================================


//==============================================================================
// Процедура установки соединения с MQTT-брокером
//==============================================================================
void nethandler_umqtt_init(struct umqtt_connection *conn)
{
  struct uip_conn *uc;
  uip_ipaddr_t ip;

  uip_ipaddr(&ip, MQTT_IP0, MQTT_IP1, MQTT_IP2, MQTT_IP3);
  uc = uip_connect(&ip, HTONS(1883));
  
  if (uc == NULL)
    return;

  conn->kalive = MQTT_KEEP_ALIVE;
  conn->clientid = (char*)MQTT_CLIENT_ID;
        
  umqtt_init(conn);
  umqtt_circ_init(&conn->txbuff);
  umqtt_circ_init(&conn->rxbuff);

  umqtt_connect(conn);
  uc->appstate.conn = conn;
}
//==============================================================================


//==============================================================================
// Функция записывает форматированную строку в буфер StrBuff
//==============================================================================
int8_t str_printf(char *StrBuff, uint8_t BuffLen, const char *args, ...)
{
  va_list ap;
  va_start(ap, args);
  int8_t len = vsnprintf(StrBuff, BuffLen, args, ap);
  va_end(ap);
  return len;
}
//==============================================================================


//==============================================================================
// MAIN
//==============================================================================
int main(void)
{
	 ecotis_port_init();
	 //Инициализируем дисплей
	 LCD_ini();
	 
  for (uint8_t  i = 0; i < 6; i++)
    uNet_eth_address.addr[i] = _eth_addr[i];
	
#if DEBUG_UIP
  printf("MAC address:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
         _eth_addr[0], _eth_addr[1], _eth_addr[2], _eth_addr[3], _eth_addr[4], _eth_addr[5]);
  printf("Init NIC...\r\n");
#endif

  // init NIC device driver
  nic_init(_eth_addr);
  ecotis_loading();
  uip_ipaddr_t ipaddr;
  uip_setethaddr(uNet_eth_address);

#if DEBUG_UIP
  printf("Init uIP...\r\n");
#endif 

  //init uIP
  uip_init();

#if DEBUG_UIP
  printf("Init ARP...\r\n");
#endif 

  //init ARP cache
  uip_arp_init();

  // init periodic timer
  clock_init();

#if (ENABLE_DHCP == 1)
  uip_ipaddr(ipaddr, 0, 0, 0, 0);
  uip_sethostaddr(ipaddr);
  uip_setnetmask(ipaddr);
  uip_setdraddr(ipaddr);

  dhcpc_init(&uNet_eth_address.addr[0], 6);
  dhcpc_request();
#else

#if DEBUG_UIP
  printf("Static IP %d.%d.%d.%d\r\n", _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
  printf("NetMask %d.%d.%d.%d\r\n", _net_mask[0], _net_mask[1], _net_mask[2], _net_mask[3]);
  printf("Gateway %d.%d.%d.%d\r\n", _gateway[0], _gateway[1], _gateway[2], _gateway[3]);
#endif

  uip_ipaddr(ipaddr, _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, _net_mask[0], _net_mask[1], _net_mask[2], _net_mask[3]);
  uip_setnetmask(ipaddr);
  uip_ipaddr(ipaddr, _gateway[0], _gateway[1], _gateway[2], _gateway[3]);
  uip_setdraddr(ipaddr);
#endif
  
  /// Стартуем программные таймеры
  // Таймер используется uIP для выполнения периодических действий с соединениями
  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  // Таймер используется uIP для обслуживания ARP
  timer_set(&arp_timer, CLOCK_SECOND * 10);
  
  // Таймер используется для уведомлеия брокера о том, что мы ещё живы 
  timer_set(&mqtt_kalive_timer, CLOCK_SECOND * MQTT_KEEP_ALIVE);
  // Таймер используется чтобы инициировать подключение к брокеру если соединение не установлено
  timer_set(&mqtt_conn_timer, CLOCK_SECOND * 3);
  // Таймер используется для периодической отправки сообщения по MQTT
  timer_set(&mqtt_pub_timer, CLOCK_SECOND * 10);
  // Таймер отрисовки ECOTIS на LCD
  timer_set(&lcd_state_timer, CLOCK_SECOND * 15);
  // Таймер отрисовки IP на LCD
  timer_set(&lcd_ip_timer, CLOCK_SECOND * 45);
  //таймер отправки показания датчика давления
  timer_set(&pressure_sensor_pub_timer, CLOCK_SECOND * 1);
  
  ADC_Init();
  
  

  while (1)
  {
    NetTask();
	system_state();
	show_state_lcd();
	show_ip_lcd();
	adc_task();

	
  }
}
//==============================================================================


//==============================================================================
// Процедура, вызываемая в основном цикле
//==============================================================================
void NetTask(void)
{
  u8_t i;

  uip_len = nic_poll();

  if (uip_len > 0)
  {
    if (BUF->type == HTONS(UIP_ETHTYPE_IP))
    {
      uip_arp_ipin();
      uip_input();

      /* If the above function invocation resulted in data that	should be 
      sent out on the network, the global variable uip_len is set to a value > 0. */
      if (uip_len > 0)
      {
	uip_arp_out();
#if UIP_SPLIT_HACK
	uip_split_output();
#elif UIP_EMPTY_PACKET_HACK
	uip_emtpy_packet_output();
#else 
	nic_send();
#endif 
      }
    }
    else if (BUF->type == HTONS(UIP_ETHTYPE_ARP))
    {
      uip_arp_arpin();
    
      /* If the above function invocation resulted in data that	should be sent
      out on the network, the global variable uip_len is set to a value > 0. */
      if (uip_len > 0)
      {
	nic_send();
      }
    }
  }
  else 
  {
    if (timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
	
      for (i = 0; i < UIP_CONNS; i++) 
      {
        uip_periodic(i);
	
        /* If the above function invocation resulted in data that	should be sent
        out on the network, the global variable uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
#if UIP_SPLIT_HACK
          uip_split_output();
#elif UIP_EMPTY_PACKET_HACK
          uip_emtpy_packet_output();
#else 
          nic_send();
#endif 
        }
      }

#if UIP_UDP
      for (i = 0; i < UIP_UDP_CONNS; i++) 
      {
        uip_udp_periodic(i);
	
        /* If the above function invocation resulted in data that should be sent
        out on the network, the global variable uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          nic_send();
        }
      }
#endif /* UIP_UDP */
		
      /* Call the ARP timer function every 10 seconds. */
      if (timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
  
    // Обрабатываем пропадание LINKа
    if (!enc28j60linkup())
    {
      uip_abort();
//      umqtt_disconnected(&mqtt);
    }
    // Отправляем KeepAlive-пакет брокеру если нужно
    if (timer_expired(&mqtt_kalive_timer))
    {
      timer_reset(&mqtt_kalive_timer);
      umqtt_ping(&mqtt);
    }
    // Устанавливаем соединение с брокером если оно не установлено
    if ((mqtt.state != UMQTT_STATE_CONNECTED) && (timer_expired(&mqtt_conn_timer)))
    {
      timer_reset(&mqtt_conn_timer);
      
      nethandler_umqtt_init(&mqtt);
      umqtt_subscribe(&mqtt, MQTT_TOPIC_ALL);
    }
    // Отправляем новое сообщение со своим IP брокеру
    if (timer_expired(&mqtt_pub_timer))
    {
	    timer_reset(&mqtt_pub_timer);
	    
	    char message[16];
	    char info[33] = "Ecotis_System_S/N:3461236123";
	    uip_gethostaddr(_ip_addr);
	    int8_t len = str_printf(message, sizeof(message), "%d.%d.%d.%d", _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
	    if (len > 0)
	    {
		    umqtt_publish(&mqtt, "/System1", (uint8_t *) message, strlen(message));
		    umqtt_publish(&mqtt, "/System1/info", (uint8_t *) info, strlen(info));
	    }
	    
    }
	if (timer_expired(&pressure_sensor_pub_timer))
	{
		timer_reset(&pressure_sensor_pub_timer);
		
		umqtt_publish(&mqtt, "/System1/pressure-sensor/value", (uint8_t *) adc_dec, strlen(adc_dec));
	}

  }

  // Ждём окончания отправки пакета. Если проходит более N мс ожидания - делаем
  // сброс логики передатчика enc28j60
  nic_send_timer = 5;
  while (nic_sending())
  {
    //    if (enc28j60Read(EIR) & EIR_TXERIF)   // Этот способ не работает, т.к. флаг ошибки передачи не устанавливается
    // Истекло время ожидания окончания передачи
    if (!nic_send_timer)
    {
      nic_txreset_num++;
      enc28j60ResetTxLogic();
    }
  }
}
//==============================================================================
