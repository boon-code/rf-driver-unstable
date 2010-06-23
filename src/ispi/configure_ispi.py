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
    cfg.choose("CS", pins, help="Chip Select (active low)")
    cfg.choose("MOSI", pins, help="Master out Slave in")
    cfg.choose("MISO", pins, help="Master in Slave out")
    cfg.choose("SCK", pins, help="Clock")

PORT = cfg.choose('PORT', pnames)
cfg.bind('DDR', set_ddr, PORT)
cfg.bind('PIN', set_pin, PORT)
cfg.bind(None, choose_pins, PORT)


# Settings (CS, 8 bit, 16 bit)
DO_USE_CS = cfg.choose("DO_USE_CS", [True, False])
DO_USE_8BIT = cfg.choose("DO_USE_8BIT", [True, False])
DO_USE_16BIT = cfg.choose("DO_USE_16BIT", [True, False])

def define_CS(obj, value):
    if value.read():
        cfg.bind("USE_CS", dummy_call, DO_USE_CS)

def define_8bit(obj, value):
    if value.read():
        cfg.bind("8BIT", dummy_call, DO_USE_CS)

def define_16bit(obj, value):
    if value.read():
        cfg.bind("16BIT", dummy_call, DO_USE_CS)

cfg.bind(None, define_CS, DO_USE_CS)
cfg.bind(None, define_8bit, DO_USE_8BIT)
cfg.bind(None, define_16bit, DO_USE_16BIT)


