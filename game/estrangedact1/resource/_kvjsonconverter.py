import os, simplejson, sys, codecs

if len(sys.argv) != 2:
	print("Hello! Please drag a file onto this one to get started with the conversion.")
	print("If the extension is .txt, it will be converted to .js, if it's .js it will be converted to .txt.");
	sys.exit(0)
	
filename, ext = os.path.splitext(sys.argv[1])

NOWT = ""

lang = filename.split('_')[1]
print("Language is %s" % lang)

if ext == '.js':
	print("Starting .js to .txt conversion.")
	kv_header = '\r\n'.join([
		'"lang"', ''
		'{',
		'\t"Language"',
		'\t"' + lang + '"',
		'\t"Tokens"',
		'\t{', ''
	])
	kv_footer = '\t}\r\n}'
	infile = filename + ext
	outfile = filename + '_js.txt'

	#O Open the original file
	with codecs.open(infile, encoding='utf-8') as f:
		lines = f.readlines()
		validlines = []
		for line in lines:
			line = line.replace("\t", NOWT)
			line = line.replace(',\r\n', '\r\n')
			if line[:1] == '"':
				validlines.append(line)
				
	# Write the new file
	with codecs.open(outfile, encoding='utf-8', mode='w+') as tf:
		tf.write(kv_header)
		for tokenline in validlines:
			tokenline = tokenline.replace(":", " ", 1)
			tf.write("\t\t%s" % tokenline)
		tf.write(kv_footer)
	print("Converted .js to .txt as file %s" % outfile)
if ext == '.txt':
	print("Starting .txt to .js conversion.")
	js_header = '\r\n'.join([
		'ae.addLanguage("' + lang + '", {',
		'\t"GameTitle": "Estranged",',
		'\t"GameSubtitle": "Act I",'
	])
	js_footer = '});'
	infile = filename + ext
	outfile = filename.replace('_js', '') + '.js'
	
	# Open the original file
	with codecs.open(infile, encoding='utf-8') as f:
		lines = f.readlines()
		validlines = []
		for line in lines:
			if line[:1] == '"' and '"\t"' in line and not "[english]" in line:
				validlines.append(line)
	
	# Write the new file
	with codecs.open(outfile, encoding='utf-8', mode='w+') as tf:
		tf.write(js_header + '\r\n')
		for tokenline in validlines:
			tokenline = tokenline.replace('"\t"', '": "', 1)
			tokenline = tokenline.replace('\r\n', '')
			tokenline = tokenline.replace('\n', '')
			tokenline = tokenline.replace('\\\\\\"', '\\"')
			tokenline = tokenline.replace('\\\\"', '\\"')
			tf.write("\t%s,\r\n" % tokenline)
		tf.write(js_footer)
	print("Converted .txt to .js as file %s" % outfile)

print("Press any key to continue...")
input()