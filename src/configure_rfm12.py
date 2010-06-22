FREQUENCY = cfg.choose("FREQUENCY", [433, 868])

def dummy_call(*args):
    return ""

def set_frequency(obj, freq):
    freq = freq.read()
    if freq == 433:
        cfg.bind("FREQUENCY_433", dummy_call, FREQUENCY)
    elif freq == 868:
        cfg.bind("FREQUENCY_868", dummy_call, FREQUENCY)

cfg.bind("BLA", set_frequency, FREQUENCY)
