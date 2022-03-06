import matplotlib.pyplot as plt 
import sys


def plot_graph():
	# x axis values 
	Dnslookup= [26546.014000000003, 143775.527, 48604.42099999999, 6917.66, 3635.367, 39703.39399999999]
	loadtime = [94372.71499999042, 131333.96899999934, 80391.9629999873, 76243.87099999876, 93809.29499999911, 58525.73999999731]
	
	DNS_requests=[172,144,93,134,226,226]
	load_time=[100681.4660000091, 60438.37099999996, 66645.21199999945, 73266.18000000599, 71798.14400000032, 134258.6089999968]


	value=1
	count=0
	percentlookup=[]
	for x in Dnslookup:
		value=(x*100)/(loadtime[count])
		percentlookup.append(value)
		count=count+1
	

	x=["Cellular_1","Cellular_2","WiFi_1","WiFi_2","WiFi_3","WiFi_4"]
	# corresponding y axis values 
	y=[73266.18,29827.08,31890.756,123182.089,76243.871,163974.822]
	plt.plot(load_time,DNS_requests,'ro')
	plt.xlabel('loadtime/ms')
	plt.ylabel('DNS_requests')

	plt.title('Impact of DNS_requests on load time')

	plt.show()

def main(argv):
	plot_graph()
	


def piechart():

	Dnslookup= [8899.867000000002, 167686.69200000007, 14350.202999999996, 13579.972000000002, 11528.831, 30754.637]
	loadtime = [26679.20500000764, 34771.281000000046, 70177.11699999927, 29827.080000002752, 26198.778000005404, 25142.861000000266]
	value=1
	count=0
	percentlookup=[]
	for x in Dnslookup:
		value=(x*100)/(loadtime[count])
		percentlookup.append(value)
		count=count+1

	data= [99,9,0,7,99,0]
	activities = ['alibaba', 'cnn','cricinfo','imdb','reddit','washingtonpost']
	colors = ['r', 'g','b','y','gold','pink']
	plt.pie(percentlookup, labels=activities, colors=colors, startangle=90, autopct='%.1f%%')
	plt.title('Cellular_2 DNS_requests')
	plt.show()

	plt.pie(loadtime, labels=activities, colors=colors, startangle=90, autopct='%.1f%%')
	plt.title('Cellular_2 loadtime')
	plt.show()




if __name__ == "__main__":
    main(sys.argv)

