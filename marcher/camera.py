import os

import pygame
import sys

from _ctypes import byref
from ctypes import create_string_buffer

from OpenGL.GL import *
from pygame.locals import *
from pygame.time import get_ticks

from voxign.marcher.function import Function
from voxign.marcher.vector import vec3


class Camera:
    def __init__(self, size, **kwargs):
        default = {"MAX_STEPS": 100,
                   "MAX_DISTANCE": 100.0,
                   "MIN_DISTANCE": 0.001,
                   "AA": 1,
                   "BACKGROUND": vec3(0.7, 0.9, 1.0),
                   "MATERIAL": vec3(0.5, 0.6, 0.6),
                   "RO": vec3(1, 1, 1),
                   "TA": vec3(0, 0, 0),
                   "LIGHT_POS": vec3(1, 4, 1),
                   "LOOK": 1}

        self.params = {**default, **kwargs}
        self.size = size

    def get_statics(self):
        s = ''
        for param, value in self.params.items():
            s += '#define ' + param + ' ' + str(value) + '\n'
        return s

    @staticmethod
    def print_log(shader):
        length = c_int()
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, byref(length))

        if length.value > 0:
            log = create_string_buffer(length.value)
            print(glGetShaderInfoLog(shader))

    def compile_shader(self, source, shader_type):
        shader = glCreateShader(shader_type)
        glShaderSource(shader, source)
        glCompileShader(shader)

        status = c_int()
        glGetShaderiv(shader, GL_COMPILE_STATUS, byref(status))
        if not status.value:
            self.print_log(shader)
            glDeleteShader(shader)
            raise ValueError('Shader compilation failed')
        return shader

    def view(self, obj):
        shader = self.compile(obj)
        self.render(shader)

    def save(self, obj, file):
        shader = self.compile(obj)
        open(file, 'w').write(shader)

    def render(self, shader):
        pygame.init()
        size = width, height = self.size
        screen_center = (size[0] / 2, size[1] / 2)
        fps = 60
        timer = 0
        screen = pygame.display.set_mode(size, DOUBLEBUF | OPENGL)
        pygame.mouse.set_visible(False)
        pygame.mouse.set_pos(screen_center)

        clock = pygame.time.Clock()

        program = glCreateProgram()

        fragment_shader = None

        fragment_shader = self.compile_shader(shader, GL_FRAGMENT_SHADER)
        glAttachShader(program, fragment_shader)

        glLinkProgram(program)

        if fragment_shader:
            glDeleteShader(fragment_shader)

        resID = glGetUniformLocation(program, "iResolution")
        mouseID = glGetUniformLocation(program, "iMouse")
        timeID = glGetUniformLocation(program, "iTime")

        glUseProgram(program)
        glUniform2fv(resID, 1, size)

        running = True
        pause = False
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    sys.exit()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_p:
                        pause = not pause
                    elif event.key == pygame.K_s:
                        pygame.image.save(screen, 'screenshot.png')
                    elif event.key == pygame.K_ESCAPE:
                        sys.exit(0)
            if not pause:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
                glUniform1f(timeID, get_ticks() / 1000)
                m = pygame.mouse.get_pos()
                glUniform2fv(mouseID, 1, (m[0], height - m[1]))
                if get_ticks() - timer > 1000:
                    print('fps', round(clock.get_fps()))
                    timer = get_ticks()
                glRecti(-1, -1, 1, 1)
                clock.tick(fps)

                pygame.display.flip()