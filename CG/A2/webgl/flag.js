onload = function() {
    var c = document.getElementById('flag');
    var gl = c.getContext('webgl') || c.getContext('experimental-webgl');
    gl.clearColor(1.0, 1.0, 1.0, 1.0);
    gl.clearDepth(1.0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    var vetexShader = createShader('vs');
    var fragmetShader = createShader('fs');
    var prg = createProgram(vetexShader, fragmetShader);

    drawArray([
        0.000,0.000,        
        0.000,0.808,        
        0.076,0.524,        
        0.150,0.600,        
        0.106,0.092,        
        0.230,0.360,        
        0.246,0.236,        
        0.372,0.284,        
        0.336,0.020,        
        0.388,-0.036,        
        0.188,-0.36,        
        0.210,-0.496,        
        0.008,-0.452,        
        0.012,-0.872,        
        0.000,-0.872,        
        -0.012,-0.872,        
        -0.008,-0.452,        
        -0.210,-0.496,        
        -0.188,-0.36,        
        -0.388,-0.036,        
        -0.336,0.020,        
        -0.372,0.284,        
        -0.246,0.236,        
        -0.230,0.360,        
        -0.106,0.092,        
        -0.150,0.600,        
        -0.076,0.524,        
        -0.000,0.808,        
        ],gl);
    drawArray([
        -1.000,1.000,        
        -0.500,1.000,        
        -0.500,-1.000,        
        -1.000,-1.000,        
        ],gl);
     drawArray([
        1.000,1.000,        
        0.500,1.000,        
        0.500,-1.000,        
        1.000,-1.000,        
        ],gl);
    gl.flush();

    function createShader(id) {
        var shader;
        var scriptElement = document.getElementById(id);

        if (!scriptElement) {
            return;
        }

        switch (scriptElement.type) {
        case 'x-shader/x-vertex':
            shader = gl.createShader(gl.VERTEX_SHADER);
            break;
        case 'x-shader/x-fragment':
            shader = gl.createShader(gl.FRAGMENT_SHADER);
            break;
        default:
            return;
        }
        gl.shaderSource(shader, scriptElement.text);
        gl.compileShader(shader);
        if (gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            return shader;
        } else {
            alert(gl.getShaderInfoLog(shader));
        }
    }

    function createProgram(vs, fs) {
        var program = gl.createProgram();
        gl.attachShader(program, vs);
        gl.attachShader(program, fs);
        gl.linkProgram(program);
        if (gl.getProgramParameter(program, gl.LINK_STATUS)) {
            gl.useProgram(program);
            return program;
        } else {
            alert(gl.getProgramInfoLog(program));
        }
    }

    function createBuffer(data) {
        var buffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
        return buffer;
    }

    function drawArray(vertex_position, gl) {
        var attLocation = gl.getAttribLocation(prg, 'position');
        var attStride = 2;
        var buffer = createBuffer(vertex_position);
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.enableVertexAttribArray(attLocation);
        gl.vertexAttribPointer(attLocation, attStride, gl.FLOAT, false, 0, 0);
        gl.drawArrays(gl.TRIANGLE_FAN, 0, vertex_position.length / attStride);
    }
};