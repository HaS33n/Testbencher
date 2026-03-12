module top(input clk, input clk2, output o2, output o);
  assign o = !clk;
  assign o2 = clk | clk2;
endmodule