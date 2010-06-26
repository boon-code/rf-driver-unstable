def dummy_call(*args):
    return ""

def set_ddr(obj, port):
    return 'DDR' + port.read().lstrip('PORT')

def set_pin(obj, port):
    return 'PIN' + port.read().lstrip('PORT')
    
def do_pins(name):
    return ["%s%d" % (name, i) for i in range(8)]

pnames = ['PORTA', 'PORTB', 'PORTC', 'PORTD']
ports = {'PORTA' : do_pins('PA'),
        'PORTB' : do_pins('PB'),
        'PORTC' : do_pins('PC'),
        'PORTD' : do_pins('PD')}

# SPI PORT and PINS
    
def choose_pins(obj, p):
    pins = ports[p.read()]
    cfg.choose("MOSI", pins, help="Master out Slave in")
    cfg.choose("MISO", pins, help="Master in Slave out")
    cfg.choose("SCK", pins, help="Clock")
    cfg.choose("CS", pins, help="Chip Select")

PORT = cfg.choose('PORT', pnames)
cfg.bind('DDR', set_ddr, PORT)
cfg.bind('PIN', set_pin, PORT)
cfg.bind(None, choose_pins, PORT)


# mutli-dev:

bla="""

all_pins = []
for (k, v) in ports.iteritems():
    all_pins.extend(v)

def check_count(value):
    try:
        num = int(value)
        if num >= 0:
            return True
    except ValueError:
        pass
    return False

def get_empty_list(obj):
    return []

def set_list(obj, *deps):
    return [i.read() for i in deps]

def set_list_list8(obj, *deps):
    elem = [i.read() for i in deps]
    cfg.bind("8DEVICES", set_list, *elem)

def set_list_list16(obj, *deps):
    elem = [i.read() for i in deps]
    cfg.bind("16DEVICES", set_list, *elem)

def choose_cs_pin16(obj, count, pt):
    n = pt.read()
    c = count.read()
    dev = "16DEV%d_" % c
    cs = cfg.choose(dev + "CS", ports[n])
    pin = cfg.bind(dev + "PIN", set_pin, pt)
    ddr = cfg.bind(dev + "DDR", set_ddr, pt)
    return cfg.bind("16DEV%d" % c, set_list, pt, ddr, pin, cs)

def do16(obj, count):
    c = count.read()
    if c <= 0:
        cfg.bind("16DEVICES", get_empty_list)
        return ""
    else:
        elem = []
        for i in range(c):
            p = cfg.choose("16DEV%d_PORT" % i, pnames)
            elem.append(cfg.bind(None, choose_cs_pin16, i, p))
        cfg.bind("16DEVICES", set_list_list16, *elem)

def choose_cs_pin8(obj, count, pt):
    n = pt.read()
    c = count.read()
    dev = "8DEV%d_" % c
    cs = cfg.choose(dev + "CS", ports[n])
    pin = cfg.bind(dev + "PIN", set_pin, pt)
    ddr = cfg.bind(dev + "DDR", set_ddr, pt)
    return cfg.bind("8DEV%d" % c, set_list, pt, ddr, pin, cs)

def do8(obj, count):
    c = count.read()
    if c <= 0:
        cfg.bind("8DEVICES", get_empty_list)
        return ""
    else:
        elem = []
        for i in range(c):
            p = cfg.choose("8DEV%d_PORT" % i, pnames)
            elem.append(cfg.bind(None, choose_cs_pin8, i, p))
        cfg.bind("8DEVICES", set_list_list8, *elem)

COUNT16 = cfg.expr("COUNT16", check=check_count,
    help="how many devices are going to use spi interface " + 
        "in 16 bit mode")
COUNT8 = cfg.expr("COUNT8", check=check_count, 
    help="how many devices are going to use spi interface " + 
        "in 8 bit mode")

cfg.bind(None, do16, COUNT16)
cfg.bind(None, do8, COUNT8)

"""
