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

