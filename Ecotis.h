#ifndef ECOTYS_H_
#define ECOTYS_H_

void ecotis_port_init(void);
void ecotis_task(void);
void ecotis_loading(void);
void system_state(void);
void load_bk_toner(void);
void load_c_toner(void);
void load_m_toner(void);
void load_y_toner(void);

#define DRIVER_PORT1  PORTC
#define DRIVER_PORT2  PORTG


#define DRIVER_PORT_DIRECT1  DDRC
#define DRIVER_PORT_DIRECT2  DDRG

#define DRIVER_PIN0  PC0
#define DRIVER_PIN1  PC1
#define DRIVER_PIN2  PC2
#define DRIVER_PIN3  PC3
#define DRIVER_PIN4  PC4
#define DRIVER_PIN5  PC5
#define DRIVER_PIN6  PC6
#define DRIVER_PIN7  PC7
#define DRIVER_PIN8  PG0
#define DRIVER_PIN9  PG1

#endif /* ECOTYS_H_ */