# << Oscilloscope GUI >>
#
# This implementaion makes use of matplotlib on Tk for agile GUI development.
#
# Reference: https://matplotlib.org/2.1.0/gallery/user_interfaces/embedding_in_tk_sgskip.html
#

import matplotlib
matplotlib.use('TkAgg')

import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import sys
import tkinter as Tk
from datetime import datetime
import time
import os

import matplotlib.pyplot as plt

import dsp
import gui
import yaml
import dataset

# Command arguments
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("port", help="serial port identifier")
parser.add_argument("-g", "--debug",
                    help="serial port identifier",
                    action="store_true")
parser.add_argument("-d", "--dataset_folder",
                    help="Data folder for saving feature data from the device",
                    default='.')
parser.add_argument("-b", "--browser",
                    help="Data browser", action="store_true")
parser.add_argument("-s", "--plot_style",
                    help="Plot style", default='dark_background')
args = parser.parse_args()

if __name__ == '__main__':

    plt.style.use(args.plot_style)

    itfc = dsp.Interface(port=args.port)
    gui = gui.GUI(interface=itfc)
        
    # Beam forming mode
    mode = dsp.ENDFIRE

    ### Default settings to DSP ###
    itfc.set_beam_forming(mode, 'c')  # ENDFIRE mode, center
    itfc.enable_pre_emphasis(True)  # Pre emphasis enabled
    ###############################

    PADX = 6
    PADX_GRID = 2
    PADY_GRID = 2
    WIDTH = 7

    CMAP_LIST = ('hot',
                 'viridis',
                 'gray',
                 'binary',
                 'ocean',
                 'magma',
                 'cubehelix',
                 'cool',
                 'winter',
                 'summer')

    ANGLE = ('L', 'l', 'c', 'r', 'R')

    cnt = 0
    repeat_action = False
    
    current_class_label = ''
    filename = None
    data = None
    cnn_model = None
    last_operation = None

    EMPTY = np.array([])

    dataset = dataset.DataSet(args.dataset_folder)
    windows, window_pos = dataset.generate_windows()
    class_file = args.dataset_folder + '/class_labels.yaml'

    if dataset.model and not args.browser:
        import inference
        cnn_model = inference.Model(class_file=class_file,
                                    model_file=args.dataset_folder + '/' + dataset.model,
                                    windows=windows)

    root = Tk.Tk()
    root.wm_title("Oscilloscope and spectrum analyzer for deep learning")

    if args.browser:
        fig, ax = plt.subplots(1, 1, figsize=(10, 4))
    else:
        fig, ax = plt.subplots(1, 1, figsize=(11, 4))        
    fig.subplots_adjust(bottom=0.15)
    
    frame = Tk.Frame(master=root)
    frame_row0 = Tk.Frame(master=frame)
    frame_row0a = Tk.Frame(master=frame_row0)
    frame_row0b = Tk.Frame(master=frame_row0, padx=PADX)
    frame_row1 = Tk.Frame(master=frame)
    frame_row2 = Tk.Frame(master=frame)
    frame_row3 = Tk.Frame(master=frame)
    frame_row4 = Tk.Frame(master=frame)
    

    canvas = FigureCanvasTkAgg(fig, master=frame_row0a)
    canvas.draw()

    # Save training data for deep learning
    def save():
        global current_class_label, cnt, filename
        class_label = entry_class_label.get()
        func, data, window, pos = last_operation
        dt = datetime.today().strftime('%Y%m%d%H%M%S')
        if args.dataset_folder:
            dataset_folder = args.dataset_folder
        else:
            dataset_folder = './data'

        if class_label == '':
            filename = dataset_folder+'/data/{}-{}'.format(func.__name__, dt)
        else:
            if func == mel_spectrogram or func == mfcc:  # Save both data at a time
                filename = dataset_folder+'/data/{}-{}-{}-{}'.format(class_label, func.__name__, pos, dt)
            else:
                filename = dataset_folder+'/data/{}-{}-{}'.format(class_label, func.__name__, dt)
            data = data.flatten()
            with open(filename+'.csv', "w") as f:
                f.write(','.join(data.astype(str)))

            if current_class_label != class_label:
                current_class_label = class_label
                cnt = 0
            cnt += 1
            counter.configure(text='({})'.format(str(cnt)))

    def repeat(func):
        global repeat_action
        if repeat_action:
            root.after(50, func)

    def infer(data, pos=0, remove_dc=False):
        probabilities = cnn_model.infer(data, remove_dc)
        class_label, p = probabilities[pos][0]
        label_inference.configure(text='This is {} ({} %)'.format(class_label, int(p)))
        
    def raw_wave(repeatable=True):
        global last_operation
        range_ = int(range_amplitude.get())
        data = gui.plot(ax, dsp.RAW_WAVE, range_=range_)
        last_operation = (raw_wave, data, None, None)
        fig.tight_layout()
        canvas.draw()
        if repeatable:
            repeat(raw_wave)

    def fft(repeatable=True):
        global last_operation
        ssub = int(spectrum_subtraction.get())
        data = gui.plot(ax, dsp.FFT, ssub=ssub)
        last_operation = (fft, data, None, None)
        fig.tight_layout()
        canvas.draw()
        if repeatable:
            repeat(fft)

    def spectrogram(data=EMPTY, pos=0, repeatable=True):
        global last_operation, windows
        ssub = int(spectrum_subtraction.get())    
        range_ = int(range_spectrogram.get())
        cmap_ = var_cmap.get()
        if data is EMPTY:
            window = windows[int(range_window.get())]
            data = gui.plot(ax, dsp.SPECTROGRAM, range_, cmap_, ssub,
                               window=window)
        else:
            window = windows[pos]
            gui.plot(ax, dsp.SPECTROGRAM, range_, cmap_, ssub, data=data,
                         window=window)
        last_operation = (spectrogram, data, window, pos)
        fig.tight_layout()
        canvas.draw()
        if repeatable:
            repeat(spectrogram)

    def mel_spectrogram(data=EMPTY, pos=0, repeatable=True):
        global last_operation, windows
        ssub = int(spectrum_subtraction.get())
        range_ = int(range_mel_spectrogram.get())
        cmap_ = var_cmap.get()
        if data is EMPTY:
            window = windows[int(range_window.get())]
            data = gui.plot(ax, dsp.MEL_SPECTROGRAM, range_, cmap_, ssub,
                               window=window)
        else:
            window = windows[pos]
            gui.plot(ax, dsp.MEL_SPECTROGRAM, range_, cmap_, ssub, data=data,
                         window=window)
        if cnn_model:
            infer(data, pos)
        last_operation = (mel_spectrogram, data, window, pos)
        fig.tight_layout()
        canvas.draw()
        if repeatable:
            repeat(mel_spectrogram)

    def mfcc(data=EMPTY, pos=0, repeatable=True):
        global last_operation, windows
        ssub = int(spectrum_subtraction.get())    
        range_ = int(range_mfcc.get())
        cmap_ = var_cmap.get()
        if data is EMPTY:
            window = windows[int(range_window.get())]
            data = gui.plot(ax, dsp.MFCC, range_, cmap_, ssub,
                               window=window, remove_dc=True)
        else:
            window = windows[pos]
            gui.plot(ax, dsp.MFCC, range_, cmap_, ssub, data=data,
                         window=window, remove_dc=True)
        if cnn_model:
            infer(data, pos, remove_dc=True)
        last_operation = (mfcc, data, window, pos)
        fig.tight_layout()
        canvas.draw()
        if repeatable:
            repeat(mfcc)

    def welch():
        global last_operation
        func, data, window = last_operation
        if func == spectrogram:
            gui.plot_welch(ax, data, window)
        else:
            print("Welch's method is for spectrogram only")
        fig.tight_layout()
        canvas.draw()

    def beam_forming(angle):
        global mode
        angle = int(angle) + 2
        itfc.set_beam_forming(mode, ANGLE[angle])

    def repeat_toggle():
        global repeat_action
        if repeat_action == True:
            repeat_action = False
            button_repeat.configure(bg='lightblue')
        else:
            repeat_action = True
            button_repeat.configure(bg='red')
            
    def pre_emphasis_toggle():
        if button_pre_emphasis.cget('bg') == 'lightblue':
            button_pre_emphasis.configure(bg='red')
            itfc.enable_pre_emphasis(True)
        else:       
            button_pre_emphasis.configure(bg='lightblue')
            itfc.enable_pre_emphasis(False)
            
    def savefig():
        global filename
        if filename:
            fig.savefig(filename+'.png')

    def remove():
        global filename, cnt
        if filename:
            os.remove(filename+'.csv')
            cnt -= 1
            counter.configure(text='({})'.format(str(cnt)))

    def quit():
        root.quit()
        root.destroy()

    def confirm():
        canvas._tkcanvas.focus_set()

    def shadow(pos):
        last_operation[0](data=last_operation[1], pos=int(pos), repeatable=False)

    def filterbank():
        data = gui.plot(ax, dsp.FILTERBANK)
        canvas.draw()

    def elapsed_time():
        gui.plot(ax, dsp.ELAPSED_TIME)

    def broadside():
        global mode
        mode = dsp.BROADSIDE
        angle = range_beam_forming.get() + 2
        itfc.set_beam_forming(mode, ANGLE[angle])

    def endfire():
        global mode
        mode = dsp.ENDFIRE
        angle = int(range_beam_forming.get()) + 2
        itfc.set_beam_forming(mode, ANGLE[angle])

    def left_mic_only():
        itfc.left_mic_only()

    def right_mic_only():
        itfc.right_mic_only()

    ### Key press event ###

    def on_key_event(event):
        c = event.key
        pos = range_window.get()
        if c == 'right':
            if pos < len(windows) - 1:
                pos += 1
                range_window.set(pos)
        elif c == 'left':
            if pos > 0:
                pos -= 1
                range_window.set(pos)            
        elif c == 'up':
            if last_operation is None:
                print('Up key becomes effective after executing an operations.')
            else:
                func = last_operation[0]
                if func in (mel_spectrogram, mfcc):
                    func(pos=int(range_window.get()), repeatable=False)
                else:
                    func(repeatable=False)
        elif c == 'down':
            save()
            
    if not args.browser:
        canvas.mpl_connect('key_press_event', on_key_event)

    ### File select event ###
    def on_select(event):
        widget = event.widget
        index = int(widget. curselection()[0])
        filename = widget.get(index)
        params = filename.split('-')
        func = globals()[params[1]]

        with open(args.dataset_folder + '/data/' + filename) as f:
            data = np.array(f.read().split(','), dtype='float')
        
        if func == mel_spectrogram or func == mfcc:
            try:
                data = data.reshape(400, dataset.filters)
                pos = int(params[2])
                func(data=data, pos=pos, repeatable=False)
            except:  # Backward compatibility (to be removed in future)
                data = data.reshape(200, dataset.filters)
                if func == mfcc:
                    empty = np.zeros((400, dataset.filters))
                    empty[200:,:] = data
                    data = empty     
                func(data=data, repeatable=False)
        else:
            data = data.reshape(200, dataset.filters)
            func(data=data, repeatable=False)
        
    ### Row 0b ####
    if args.browser:
        list_files = Tk.Listbox(master=frame_row0b, width=30, height=20)
        files = [f for f in os.listdir(args.dataset_folder+'/data')]
        for f in files:
            list_files.insert(Tk.END, f)
        list_files.bind('<<ListboxSelect>>', on_select)
    
        scrollbar = Tk.Scrollbar(master=frame_row0b, orient="vertical")
        scrollbar.config(command=list_files.yview)
        list_files.config(yscrollcommand=scrollbar.set)
    
    ### Row 1 ####
    entry_class_label = Tk.Entry(master=frame_row1, width=14)
    var_cmap = Tk.StringVar()
    var_cmap.set('hot')
    cmap = Tk.OptionMenu(frame_row1, var_cmap, *CMAP_LIST)
    counter = Tk.Label(master=frame_row1)
    counter.configure(text='({})'.format(str(0)))
    range_amplitude = Tk.Spinbox(master=frame_row1, width=6,
                                 values=[2**8, 2**9, 2**11, 2**13, 2**15])
    range_mel_spectrogram = Tk.Spinbox(master=frame_row1, width=3,
                                       values=[dsp.NUM_FILTERS, int(dsp.NUM_FILTERS*.8), int(dsp.NUM_FILTERS*0.6)])
    range_spectrogram = Tk.Spinbox(master=frame_row1, width=4,
                                   values=[int(dsp.NN/2), int(dsp.NN/2.0*.7), int(dsp.NN/2.0*0.4)])
    range_mfcc = Tk.Spinbox(master=frame_row1, width=3,
                            values=[13, 20])
    spectrum_subtraction = Tk.Spinbox(master=frame_row1, width=3,
                                      values=[0, 10, 15, 20, 25])
    label_class = Tk.Label(master=frame_row1, text='Class label:')
    label_image = Tk.Label(master=frame_row1, text='Subtraction:')
    label_color = Tk.Label(master=frame_row1, text='Color:')
    button_waveform = Tk.Button(master=frame_row1, text='Wave', command=raw_wave,
                                bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_psd = Tk.Button(master=frame_row1, text='FFT', command=fft,
                           bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_spectrogram = Tk.Button(master=frame_row1, text='Spec', command=spectrogram,
                                   bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_mel_spectrogram = Tk.Button(master=frame_row1, text='Mel spec', command=mel_spectrogram,
                                       bg='pink', activebackground='grey', padx=PADX, width=WIDTH)
    button_mfcc = Tk.Button(master=frame_row1, text='MFCCs', command=mfcc,
                            bg='yellowgreen', activebackground='grey', padx=PADX, width=WIDTH)

    ### Row 2 ####
    button_repeat = Tk.Button(master=frame_row2, text='Repeat', command=repeat_toggle,
                              bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_pre_emphasis = Tk.Button(master=frame_row2, text='Emphasis', command=pre_emphasis_toggle,
                                    bg='red', activebackground='grey', padx=PADX, width=WIDTH)
    button_savefig = Tk.Button(master=frame_row2, text='Savefig', command=savefig,
                               bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_remove = Tk.Button(master=frame_row2, text='Remove', command=remove,
                              bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_save = Tk.Button(master=frame_row2, text='Save', command=save,
                              bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_quit = Tk.Button(master=frame_row2, text='Quit', command=quit,
                            bg='yellow', activebackground='grey', padx=PADX, width=WIDTH)
    label_beam_forming = Tk.Label(master=frame_row2, text='Beam forming:')
    label_left = Tk.Label(master=frame_row2, text='L')
    label_right = Tk.Label(master=frame_row2, text='R')
    range_beam_forming = Tk.Scale(master=frame_row2, orient=Tk.HORIZONTAL, length=70,
                                  from_=-1, to=1, showvalue=0, command=beam_forming)
    button_confirm = Tk.Button(master=frame_row2, text='Confirm', command=confirm,
                            bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)
    button_welch = Tk.Button(master=frame_row2, text='Welch', command=welch,
                            bg='lightblue', activebackground='grey', padx=PADX, width=WIDTH)

    ### Row 3 ####
    button_filterbank = Tk.Button(master=frame_row3, text='Filterbank', command=filterbank,
                                  bg='lightblue', activebackground='grey', padx=PADX)
    button_elapsed_time = Tk.Button(master=frame_row3, text='Elapsed time', command=elapsed_time,
                                    bg='lightblue', activebackground='grey', padx=PADX)
    button_broadside = Tk.Button(master=frame_row3, text='Broadside', command=broadside,
                                 bg='lightblue', activebackground='grey', padx=PADX)
    button_endfire = Tk.Button(master=frame_row3, text='Endfire', command=endfire,
                               bg='lightblue', activebackground='grey', padx=PADX)
    button_left_mic_only = Tk.Button(master=frame_row3, text='Left mic only', command=left_mic_only,
                                     bg='lightblue', activebackground='grey', padx=PADX)
    button_right_mic_only = Tk.Button(master=frame_row3, text='Right mic only', command=right_mic_only,
                                      bg='lightblue', activebackground='grey', padx=PADX)

    ### Row 4 ####
    label_window = Tk.Label(master=frame_row4, text='Window:')
    range_window = Tk.Scale(master=frame_row4, orient=Tk.HORIZONTAL, length=120,
                                from_=0, to=len(windows)-1, command=shadow, tickinterval=1)
    if cnn_model:
        label_inference = Tk.Label(master=frame_row4, width=40, fg='DeepSkyBlue4', padx=PADX)
        label_inference.config(font=("Arial", 20))
    
    ##### Place the parts on Tk #####

    frame.pack(expand=True, fill=Tk.BOTH)

    ### Row 0: main canvas
    if args.browser:
        frame_row0a.grid(row=0, column=0)
        frame_row0b.grid(row=0, column=1)
        list_files.grid(row=0, column=0, padx=PADX_GRID)
        list_files.pack(side="left", expand=True, fill=Tk.BOTH)
        scrollbar.pack(side="right", expand=True, fill=Tk.BOTH)
    else:
        frame_row0a.pack(expand=True, fill=Tk.BOTH)
    frame_row0.pack(expand=True, fill=Tk.BOTH)
    canvas._tkcanvas.pack(expand=True, fill=Tk.BOTH)

    ### Row 1: operation ####

    frame_row1.pack(pady=PADY_GRID)

    if not cnn_model:

        # Class label entry
        label_class.grid(row=0, column=0, padx=PADX_GRID)
        entry_class_label.grid(row=0, column=1, padx=PADX_GRID)
        counter.grid(row=0, column=2, padx=PADX_GRID)

        # Waveform
        range_amplitude.grid(row=0, column=3, padx=PADX_GRID)
        button_waveform.grid(row=0, column=4, padx=PADX_GRID)

        # FFT (PSD)
        button_psd.grid(row=0, column=5, padx=PADX_GRID)

        # Linear-scale Spectrogram (PSD)
        range_spectrogram.grid(row=0, column=6, padx=PADX_GRID)
        button_spectrogram.grid(row=0, column=7, padx=PADX_GRID)

    if not cnn_model or (cnn_model and dataset.feature == 'mel_spectrogram'):
        # Mel-scale Spectrogram (PSD)
        range_mel_spectrogram.grid(row=0, column=8, padx=PADX_GRID)
        button_mel_spectrogram.grid(row=0, column=9, padx=PADX_GRID)

    # MFCC
    if not cnn_model or (cnn_model and dataset.feature == 'mfcc'):
        range_mfcc.grid(row=0, column=10, padx=PADX_GRID)
        button_mfcc.grid(row=0, column=11, padx=PADX_GRID)

    # CMAP
    label_image.grid(row=0, column=12, padx=PADX_GRID)
    label_image.grid(row=0, column=13, padx=PADX_GRID)
    spectrum_subtraction.grid(row=0, column=14, padx=PADX_GRID)
    cmap.grid(row=0, column=15, padx=PADX_GRID)

    ### Row 2 ####

    frame_row2.pack(pady=PADY_GRID)

    # Beam forming
    label_beam_forming.grid(row=0, column=0, padx=PADX_GRID)
    label_left.grid(row=0, column=1, padx=PADX_GRID)
    range_beam_forming.grid(row=0, column=2, padx=PADX_GRID)
    label_right.grid(row=0, column=3, padx=PADX_GRID)

    # Repeat, pre_emphasis, save fig and delete
    button_repeat.grid(row=0, column=4, padx=PADX_GRID)
    button_pre_emphasis.grid(row=0, column=5, padx=PADX_GRID)
    if not cnn_model:
        button_welch.grid(row=0, column=6, padx=PADX_GRID)
        button_confirm.grid(row=0, column=7, padx=PADX_GRID)
        button_save.grid(row=0, column=8, padx=PADX_GRID)
        button_remove.grid(row=0, column=9, padx=PADX_GRID)
    button_savefig.grid(row=0, column=10, padx=PADX_GRID)
        
    # Quit
    button_quit.grid(row=0, column=10, padx=PADX_GRID)

    ### Row 3 ####

    # DEBUG
    if args.debug:
        frame_row3.pack(pady=PADY_GRID)
        button_filterbank.grid(row=0, column=0, padx=PADX_GRID)
        button_elapsed_time.grid(row=0, column=1, padx=PADX_GRID)
        button_broadside.grid(row=0, column=2, padx=PADX_GRID)    
        button_endfire.grid(row=0, column=3, padx=PADX_GRID)            
        button_left_mic_only.grid(row=0, column=4, padx=PADX_GRID)    
        button_right_mic_only.grid(row=0, column=5, padx=PADX_GRID)    

    ### Row 4 ####
    frame_row4.pack(pady=PADY_GRID)
    label_window.grid(row=0, column=0, padx=PADX_GRID)
    range_window.grid(row=0, column=1, padx=PADX_GRID)
    if cnn_model:
        label_inference.grid(row=0, column=2, padx=PADX_GRID)
        label_inference.configure(text='...')

    ##### loop forever #####
    Tk.mainloop()
