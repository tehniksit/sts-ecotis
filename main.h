#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <timers.h>
#include <avr/io.h>
#include <util/delay.h>
#if (ENABLE_DHCP == 1)
#include <dhcpc.h>
#endif
#include "adc.h"

#include "umqtt.h"
#include "enc28j60.h"
#include "clock.h"
#include "uip.h"
#include "uip_arp.h"
#include "nic.h"
#include "main.h"
#include "net_config.h"
#include "terminal_io.h"

#include "lcd.h"
#include "Ecotis.h"

#if UIP_SPLIT_HACK
#include "uip-split.h"
#elif UIP_EMPTY_PACKET_HACK
#include "uip-empty-packet.h"
#endif



#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#define MQTT_KEEP_ALIVE			30
#define MQTT_CLIENT_ID			"enc28j60_avr_client"
#define MQTT_TOPIC_ALL			"/#"
#define MQTT_IP0                        5
#define MQTT_IP1                        45
#define MQTT_IP2                        114
#define MQTT_IP3                        8//46


#define MQTT_TOPIC_TONER_SUPPLY1			"/System1/toner_supply1"
#define MQTT_TOPIC_TONER_SUPPLY2			"/System1/toner_supply2"
#define MQTT_TOPIC_TONER_SUPPLY3			"/System1/toner_supply3"
#define MQTT_TOPIC_TONER_SUPPLY4			"/System1/toner_supply4"


uint8_t toner_bk_count = 0;
uint8_t toner_c_count = 0;
uint8_t toner_m_count = 0;
uint8_t toner_y_count = 0;



struct timer periodic_timer;
struct timer arp_timer;
struct timer mqtt_kalive_timer;
struct timer mqtt_conn_timer;
struct timer mqtt_pub_timer;
struct timer lcd_state_timer;
struct timer lcd_ip_timer;

static clock_time_t timerCounter = 0;

static struct uip_eth_addr uNet_eth_address;

static uint8_t mqtt_txbuff[150];
static uint8_t mqtt_rxbuff[200];

static inline uint16_t htons(uint16_t x);

static void handle_message(struct umqtt_connection *conn, char *topic, char *data);

static struct umqtt_connection mqtt =
{
	.txbuff =
	{
		.start = mqtt_txbuff,
		.length = sizeof(mqtt_txbuff),
	},
	.rxbuff =
	{
		.start = mqtt_rxbuff,
		.length = sizeof(mqtt_rxbuff),
	},
	.message_callback = handle_message,
};

uint8_t nic_send_timer;
uint16_t nic_txreset_num = 0;

// TCP/IP parameters in data memory
uint8_t _eth_addr[6] = {ETHADDR0, ETHADDR1, ETHADDR2, ETHADDR3, ETHADDR4, ETHADDR5};
uint8_t _ip_addr[4] = {IPADDR0, IPADDR1, IPADDR2, IPADDR3};
uint8_t _net_mask[4] = {NETMASK0, NETMASK1, NETMASK2, NETMASK3};
uint8_t _gateway[4] = {DRIPADDR0, DRIPADDR1, DRIPADDR2, DRIPADDR3};


void NetTask(void);
//static inline uint16_t htons(uint16_t x);
#endif