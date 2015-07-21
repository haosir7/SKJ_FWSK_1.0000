#ifndef MODEM_ERROR_H
#define MODEM_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

//error coding format
#define MODEM_ERROR_DOMAIN_SHIFT	12
#define MODEM_ERROR_ID_SHIFT		0
#define MODEM_ERROR_DOMAIN_MASK		0xFF
#define MODEM_ERROR_ID_MASK			0xFFF

#define MODEM_ERROR_DOMAIN( value ) (int)(((int)value >> MODEM_ERROR_DOMAIN_SHIFT) & \
	( MODEM_ERROR_DOMAIN_MASK ))
#define MODEM_ERROR_ID( value )     (int)(((int)value >> MODEM_ERROR_ID_SHIFT) & \
	( MODEM_ERROR_ID_MASK ))

#define MODEM_ERROR_NUM( domain,id ) (int)((((int)domain & MODEM_ERROR_DOMAIN_MASK) << MODEM_ERROR_DOMAIN_SHIFT) | \
		(((int)id & MODEM_ERROR_ID_MASK) << MODEM_ERROR_ID_SHIFT))


//errorno definition

//Device error domain
#define MODEM_ERROR_DEVICE			1
//CME error domain
#define MODEM_ERROR_CME				2
//CMS error domain
#define MODEM_ERROR_CMS				3
//AT command error domain
#define MODEM_ERROR_AT				4
//WIPSOFT error domain
#define MODEM_ERROR_WIPSOFT			5
//WMMP error domain
#define MODEM_ERROR_WMMP			6



#ifdef __cplusplus
}
#endif

#endif
