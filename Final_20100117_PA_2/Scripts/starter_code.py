import sys
import json as js
import json
from haralyzer import HarParser, HarPage
import glob	
import simplejson
import matplotlib.pyplot as plt
 

def main(argv):
	#filter_request(argv)
	 #piechart()
	 dns_requests(argv)




	

		    # returns for each




def dns_requests(argv):
	sitename = str(argv[1])
	print sitename
	parsed = js.load(open(sitename+'.har'))['log']
	f=open("dns_requests.txt","a")
	y=0

	total_size = 0
	for entry in parsed['entries']:
		if entry['response']['bodySize'] > 0:
			total_size += entry['response']['bodySize']

	total_requests=0
	all_entries=[]
	for entry in parsed['entries']:
		request=entry['request']
		url=request['url']
		if 'reddit' not in url:
			all_entries.append(entry)
			total_requests =total_requests+1

	

	print total_requests 
		
	line=''
	line=str(total_requests)+" "+" "+sitename
	f.write(line+"\n")


		
def dnslookup(argv):
	sitename = str(argv[1])
	print sitename
	dns_look_up=[]
	f=open("dnslookup.txt","a")
	for file in glob.glob('*.har'):
		parsed = js.load(open(file))['log']
		print len(parsed['entries']), len(parsed['pages'])
		print parsed['entries'][0].keys()
		print parsed['pages'][0].keys()
		entries=parsed['entries']
		total_dns_time=0
		for each in entries:
			timings=each['timings']
			dns=timings['dns']
			if dns!=-1:
				total_dns_time=total_dns_time+dns

		
		dns_look_up.append(total_dns_time)
		line=str(total_dns_time)+" "+ file
		f.write(line +"\n")

			
		print total_dns_time
	simplejson.dump(dns_look_up,f)
	f.close()

def piechart():
	data= [99,9,0,7,99,0]
	activities = ['cnn', 'imdb','reddit','cricinfo','washingtonpost','alibaba']
	colors = ['r', 'g','b','y','gold','pink']
	plt.pie(data, labels=activities, colors=colors, startangle=90, autopct='%.1f%%')
	plt.title('washingtonpost')
	plt.show()


def filter_request(argv):
	
	sitename = str(argv[1])
	print sitename
	parsed = js.load(open(sitename+'.har'))['log']
	f=open("filterequests.txt","a")
	print len(parsed['entries']), len(parsed['pages'])
	print parsed['entries'][0].keys()
	print parsed['pages'][0].keys()
	y=0

	total_size = 0
	for entry in parsed['entries']:
		if entry['response']['bodySize'] > 0:
			total_size += entry['response']['bodySize']

	size=0
	all_entries=[]
	for entry in parsed['entries']:
		request=entry['request']
		url=request['url']
		if 'washingtonpost' not in url:
			 all_entries.append(entry)
			 size += entry['response']['bodySize']
	
	percent=(size*100)/total_size	

	print size 
	print total_size
	
		
	line=''
	line=str(size)+" "+str(total_size)+" "+ str(percent)+" "+sitename
	f.write(line+"\n")


def content_type():
	image_size=[]
	page_load=[]
	for file in glob.glob('*.har'):
		
		sitename = str(file)
		parsed = js.load(open(sitename))['log']

		print len(parsed['entries']), len(parsed['pages'])
		print parsed['entries'][0].keys()
		print parsed['pages'][0].keys()


		with(open(sitename, 'r')) as f:
			print "website is "+sitename
			har_parser=HarParser(json.loads(f.read()))
			for entry in har_parser.har_data['entries']:
				if 'pageref' not in entry:
					entry['pageref'] = "asd"

			for page in har_parser.pages:	
				assert isinstance(page, HarPage)
				f=open("loadvsimagesize.txt","a")

				print "Page size :" + str(page.page_size_trans/1000)+ " kb"
				print "Page_Load_time is "+str(page.page_load_time)+" ms"
				image_size.append(page.page_size_trans)
				page_load.append(page.page_load_time)
				image_percent=(page.image_size_trans*100)/(page.page_size_trans)
				css_percent=(page.css_size_trans*100)/(page.page_size_trans)
				text_percent=(page.text_size_trans*100)/(page.page_size_trans)
				js_percent=(page.js_size_trans*100)/(page.page_size_trans)
				audio_percent=(page.audio_size_trans*100)/(page.page_size_trans)
				video_percent=(page.video_size_trans*100)/(page.page_size_trans)
				line=str(image_percent)+" "+str(css_percent)+" "+str(text_percent)+" "+str(js_percent)+" "+str(audio_percent)+" "+str(video_percent)+" "+str(page.page_load_time)+ " "+sitename
				f.write(line +"\n")
				##print "Image size : " + str(page.image_size)+ " bytes"
				##print "Image_size_trans : " + str(page.image_size_trans)+ " bytes"
				##print "Image_Load_time is "+str(page.image_load_time)+" ms"
				##print "JSS size : " + str(page.js_size)+ " bytes"
				##print "JSS_Load_time is "+str(page.js_load_time)+" ms"
	f=open("loadvsimagesize.txt","a")
	simplejson.dump(image_size,f)
	f.write("\n")
	simplejson.dump(page_load,f)
	f.close




def image_sizevsloadtime():
	image_size=[]
	page_load=[]
	for file in glob.glob('*.har'):
		
		sitename = str(file)
		parsed = js.load(open(sitename))['log']

		print len(parsed['entries']), len(parsed['pages'])
		print parsed['entries'][0].keys()
		print parsed['pages'][0].keys()


		with(open(sitename, 'r')) as f:
			print "website is "+sitename
			har_parser=HarParser(json.loads(f.read()))
			for entry in har_parser.har_data['entries']:
				if 'pageref' not in entry:
					entry['pageref'] = "unknow"

			for page in har_parser.pages:	
				assert isinstance(page, HarPage)
				f=open("loadvsimagesize.txt","a")

				print "Page size :" + str(page.page_size_trans/1000)+ " kb"
				print "Page_Load_time is "+str(page.page_load_time)+" ms"
				image_size.append(page.page_size_trans)
				page_load.append(page.page_load_time)
				line=str((page.audio_size*100/(page.page_size_trans)))+" "+str(page.page_load_time) + " " +sitename
				f.write(line +"\n")
				##print "Image size : " + str(page.image_size)+ " bytes"
				##print "Image_size_trans : " + str(page.image_size_trans)+ " bytes"
				##print "Image_Load_time is "+str(page.image_load_time)+" ms"
				##print "JSS size : " + str(page.js_size)+ " bytes"
				##print "JSS_Load_time is "+str(page.js_load_time)+" ms"
	f=open("loadvsimagesize.txt","a")
	simplejson.dump(image_size,f)
	f.write("\n")
	simplejson.dump(page_load,f)
	f.close


def pagesizevsload():
	page_size=[]
	page_load=[]
	for file in glob.glob('*.har'):
		
		sitename = str(file)
		parsed = js.load(open(sitename))['log']

		print len(parsed['entries']), len(parsed['pages'])
		print parsed['entries'][0].keys()
		print parsed['pages'][0].keys()


		with(open(sitename, 'r')) as f:
			print "website is "+sitename
			har_parser=HarParser(json.loads(f.read()))
			for entry in har_parser.har_data['entries']:
				if 'pageref' not in entry:
					entry['pageref'] = "unknow"

			for page in har_parser.pages:	
				assert isinstance(page, HarPage)
				f=open("loadvssize.txt","a")

				print "Page size :" + str(page.page_size/1000)+ " kb"
				print "Page_Load_time is "+str(page.page_load_time)+" ms"
				page_size.append(page.page_size)
				page_load.append(page.page_load_time)
				line=str(page.page_size)+" "+str(page.page_load_time) + " " +sitename
				f.write(line +"\n")
				##print "Image size : " + str(page.image_size)+ " bytes"
				##print "Image_size_trans : " + str(page.image_size_trans)+ " bytes"
				##print "Image_Load_time is "+str(page.image_load_time)+" ms"
				##print "JSS size : " + str(page.js_size)+ " bytes"
				##print "JSS_Load_time is "+str(page.js_load_time)+" ms"
	f=open("loadvssize.txt","a")
	simplejson.dump(page_size,f)
	f.write("\n")
	simplejson.dump(page_load,f)
	f.close



if __name__ == "__main__":
    main(sys.argv)

