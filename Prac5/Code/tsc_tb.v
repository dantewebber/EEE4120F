// TSC Test Bench

module tsc_tb;

    // Var declaration
    integer count;

    // Parameters
    parameter CLK_PERIOD = 10; // Clock period in ns

    // Signals
    reg rst, start, clk, sbf;
    wire trd, cd;
    wire [8:0] sd;

    // Instantiate the TSC module
    TSC tsc_inst (
        .rst(rst),
        .start(start),
        .clk(clk),
        .sbf(sbf),
        .trd(trd),
        .sd(sd),
        .cd(cd)
    );

    // Clock generation
    // always #((CLK_PERIOD)/2) clk = ~clk;

    initial begin
        // 100ns clock period
        clk = 1'b0;
        forever #50 clk = ~clk;
    end

    initial begin
        $dumpfile("trigger_test.vcd");
        $dumpvars(0,tsc_tb);
    end


    // Stimulus
    initial begin
        // Push reset line high to reset TSC module
        rst = 1; 
        start = 0; 

        #20 rst = 0; // Deassert reset

        // Start the TSC by pushing the start line high
        #100 start = 1;
        #100 start = 0;

        // Wait until the tigger is detected
        @(posedge trd);
        $display("Test Bench captured trigger event.");
        
        // Checks if the current timer value is stored in the TRIGTM register on a trigger event
        if (tsc_inst.trigtm == 0) begin
            $display("ERROR: TRIGTM not captured");
        end else begin
            $display("TRIGTM: %d", tsc_inst.trigtm);
        end

        // Send buffer request
        #100 sbf = 1;
        @(posedge sd);
        sbf = 0;

        // @(posedge sd);

        // Print the data being sent on the SD line for 32 bytes,
        // if done correctly, a new value should be sent every 
        // positive clock cycle, so this should print out 32
        // 'different' values.
        for (count = 0; count < 32; count = count +1)
        begin 
            @(posedge clk);
            $display("SD Data: %b", {sd[8], sd[7:0]});
            $display("SD Data (in decimal): %d", sd);
        end

        // Wait for the data to finish sending
        @(posedge cd);
        $display("Test bench received buffer send completed");

        // Display the data sent in sd
        // $display("SD Data: %b", {sd[8], sd[7:0]});

        #2000 $finish;
    end

endmodule