import microstacknode.hardware.gps.l80gps
import time

gps = microstacknode.hardware.gps.l80gps.L80GPS()
try:
	gps.locus_query()
except:
	pass
gps.locus_start()
time.sleep(15)
try:
	print(gps.locus_query_data())
except:
	pass
gps.locus_stop()
gps.locus_erase()
