var MIN_SIGNAL = 1000;
var MAX_SIGNAL = 2000;
var MIDDLE_SIGNAL = 1500;
var SPEED = 0.5;


//Smooth tilt beta version fully functional JMMaupin 2014-09-21 
var SliderPos = 0;		// Internal value from Input Slider desired position
var SliderAcc = 0;		// Smooth ghost from SliderPos
var DeltaSldAcc = 0;
var AbsDeltaSldAcc = 0;
var DirSliderAcc = 0;	// Direction from SliderPos
var CurrentPos = 0;		// Live out position
var DeltaPos = 0;
var DeltaPosR2 = 0;		// DeltaPosSqr
var AbsDeltaPos = 0;
var DirectPos = 0;		// Direction

// Behaviour parameters. Results depend from the running cycle time approximately every 10ms (Mesured)
var AccRatio = 1/50 	// Impact acceleration
var SpeedRatio =  1/2   // Impact speed and deceleration (High speed is clipped by MaxSpeed)
var SExtSpeed = 0	    // Internal value from Input Slider or Switch speed profile
var MaxSpeed = 100      // Clipped the high speed

// Inputs HMI ----------------------------------
//local InSlider = "ch10"		-- Input for desired position source device
//local ExternalSpeed = "ch30"  -- Input for desired Global speed source device


function loop() {
	var inSignal = getSignal();
	var outSignal = generateSignal(inSignal);
	
	var angle = mapfloat(outSignal, MIN_SIGNAL, MAX_SIGNAL, 0, 90);
	document.getElementById('camera').style.transform = 'rotate('+angle+'deg)';
	document.getElementById('output').textContent = "PWM IN: " + inSignal + "    PWM OUT: " + outSignal;

	setTimeout(function() { loop(); }, 10);
}

function mapfloat(x, in_min, in_max, out_min, out_max)
{
	return parseFloat(x - in_min) * (out_max - out_min) / parseFloat(in_max - in_min) + out_min;
}

function generateSignal(position) {
	SliderPos = position; // Get the Desired target position
	// Compute the desired global speed
	SExtSpeed = SPEED; //(1024 + speed / 2) /1024; // 0.5 for -1024 | 1 for 0 | 1.5 for 1024
	
	// 1-st smooth the Slider acceleration and speed
	DeltaSldAcc = SliderAcc - SliderPos;		// Difference between Slider and internal slider smooth acc
	AbsDeltaSldAcc=Math.abs(DeltaSldAcc);		// Absolute value
	DirSliderAcc = DeltaSldAcc > 0 ? 1 : -1;	// What is the direction ?
	// Cyclic Smooth slider computing
	SliderAcc = SliderAcc - DirSliderAcc * AbsDeltaSldAcc * AccRatio * SExtSpeed;
	// End smooth Slider
	//---------------------------------------------------------------------------------
	
	// 2-nd Compute final position
	DeltaPos = CurrentPos - SliderAcc;		// Difference between Slider smooth and internal and Output position
	AbsDeltaPos = Math.abs(DeltaPos);		// Absolute value
	DirectPos = DeltaPos > 0 ? 1 : -1;		// What is the direction ?
	DeltaPosR2 = Math.sqrt(AbsDeltaPos);	// Root square from AbsDeltaPos to shorten the deceleration
	if (DeltaPosR2 > MaxSpeed) {			// When DeltaPosR2 is high final speed is too high then
		DeltaPosR2 = MaxSpeed;				// Clipped DeltaPosR2
	}
	// Cyclic final output position computing
	CurrentPos = CurrentPos - DirectPos * DeltaPosR2 * SpeedRatio * SExtSpeed;
	// End Compute final position
	
	// Function return results
	//return CurrentPos, SliderAcc, DeltaPos, SExtSpeed * 10.24

	return CurrentPos;
}

function getSignal() {
	var signal = parseInt(document.getElementById('slider').value);
	if (signal == undefined) signal = 1000;
	return signal;
}

loop();