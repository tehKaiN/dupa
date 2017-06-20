% function dupa()
	clear; clc;
	% Free all COM ports
	instrreset;
	
	% Establish connection with DUPA
	s = serial('COM3', 'Terminator', 10, 'Timeout', 30);
	fopen(s);
	if(~isvalid(s))
		disp 'Invalid serial port!';
		return
	end
	java.lang.Thread.sleep(5000);
	
	% Send scan request
	fprintf(s, 'readBetter\n');
	wasEnd = 0;
	lines = {};
	while ~wasEnd
		line = fgetl(s);
		if strcmp(line,'end')
			wasEnd = 1;
		else
			lines{end+1} = line;
		end
	end
	fclose(s);
	disp('Done reading');
	
	% Parse data
	steps = [];
	vals = [];
	for i = 1:numel(lines)
		parsed = strsplit(lines{i}, ';');
		steps(end+1) = str2double(parsed{1});
		vals(end+1) = str2double(parsed{2});
	end
	dist = 5*vals/1024;
	p = sharp;
	plot(steps, polyval(p, dist));
	xlabel('krok');
	ylabel('dystans');
	
	% 240mm na przestrzeni 350 krokow
	% ksiazka Carrolla: 127mm
	% kroki: 283-93 = 193
	% 193/350 * 240 = 132mm (4% bledu!)
% end