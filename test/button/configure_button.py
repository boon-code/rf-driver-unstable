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

COUNT = cfg.expr("COUNT", check=check_count)
PORT = cfg.choose("PORT", pnames)

cfg.bind("DDR", set_ddr, PORT)
cfg.bind("PIN", set_pin, PORT)

cfg.bind(None, choose_pins, PORT, COUNT)
