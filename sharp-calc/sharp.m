function sharp
	clear; clc;
	L = [4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 25, 30];
	V = [2.75, 2.35, 2.0, 1.77, 1.55, 1.4, 1.28, 1.08, 0.95, 0.8, 0.75, 0.65, 0.52, 0.45];
	plot(L, V);
	for n = 2:5
		p(n,1:n+1) = polyfit(L, V, n);
		e(n) = sum((polyval(p(n,1:n+1), L) - V).^2);
	end
	% Polyfit zwraca wspolczynniki wielomianu zaczynajac od tego stojacego
	% przy najwyzszej potedze.
	hold on;
	n = 5;
	poly = p(n,1:n+1);
	L = linspace(4, 30, 100);
	plot(L, polyval(poly, L));
	hold off;
	legend('V', 'Vmod');
	format longEng
	disp(poly);
end