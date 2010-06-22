def set_ddr(obj, port):
    return 'DDR' + port.read().lstrip('PORT')

def set_pin(obj, port):
    return 'PIN' + port.read().lstrip('PORT')
    
def do_pins(name):
    return ["%s%d" % (name, i) for i in range(8)]

pnames = ['PORTA', 'PORTB', 'PORTC', 'PORTD']
ports = {"PORTA" : do_pins("PA"),
        "PORTB" : do_pins("PB"),
        "PORTC" : do_pins("PC"),
        "PORTD" : do_pins("PD")}

def check_count(value):
    try:
        num = int(value)
        if 0 < num <= 8:
            return True
    except ValueError:
        pass
    return False

def check_delay(value):
    try:
        num = int(value)
        if 0 < num < 1000:
            return True
    except ValueError:
        pass
    return False

def set_pins_list(obj, *pins):
    return [i.read() for i in pins]

def set_mask(obj, pins):
    pins = pins.read()
    return "(" + " | ".join(["_BV(" + i + ")" for i in pins]) + ")"

def choose_pins(obj, p, c):
    pins = ports[p.read()]
    pl = []
    for i in range(c.read()):
        pl.append(cfg.choose("PIN_%s" % i, pins))
    PINS = cfg.bind("PINS", set_pins_list, *pl)
    cfg.bind("MASK", set_mask, PINS)

def inter_flags(obj, intr):
    return "INTF" + intr.read().lstrip("INT")

def get_port_from_int(obj, intr):
    intr = intr.read()
    if intr == "INT0":
        return "PORTD"
    elif intr == "INT1":
        return "PORTD"
    elif intr == "INT2":
        return "PORTB"

def get_pin_from_int(obj, intr):
    intr = intr.read()
    if intr == "INT0":
        return "PD2"
    elif intr == "INT1":
        return "PD3"
    elif intr == "INT2":
        return "PB2"

def int_settings(obj, intr):
    intr = intr.read()
    if intr == "INT0":
        return "cbi(MCUCR, ISC00);cbi(MCUCR, ISC01);"
    elif intr == "INT1":
        return "cbi(MCUCR, ISC10);cbi(MCUCR, ISC11);"
    elif intr == "INT2":
        return "cbi(MCUCSR, ISC2);"

COUNT = cfg.expr("COUNT", check=check_count)
PORT = cfg.choose("PORT", pnames)

cfg.bind("DDR", set_ddr, PORT)
cfg.bind("PIN", set_pin, PORT)

INTERRUPT = cfg.choose("INTERRUPT", ["INT0", "INT1", "INT2"])
cfg.bind("INT_FLAG", inter_flags, INTERRUPT)

cfg.expr("READ_DELAY", help="short delay (us) between setting DDR" + 
                            " and reading in value of pin",
                            check=check_delay)

INT_PORT = cfg.bind("INT_PORT", get_port_from_int, INTERRUPT)
cfg.bind("INT_PIN", set_pin, INT_PORT)
cfg.bind("INT_DDR", set_ddr, INT_PORT)
cfg.bind("PINT", get_pin_from_int, INTERRUPT)
cfg.bind("SET_INT_SENSE", int_settings, INTERRUPT)

cfg.bind(None, choose_pins, PORT, COUNT)

