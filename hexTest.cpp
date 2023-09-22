#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <algorithm>

using namespace std;

//// Function to convert a hex string to an integer (reading two characters at a time)
int hexStringToInt(const std::string& hexStr) {
    int value;
    std::stringstream converter;
    converter << std::hex << hexStr;
    converter >> value;
    return value;
}

// Function to convert a hex string to ASCII
char hexStringToASCII(const std::string& hexStr) {
    int intValue;
    std::stringstream converter;
    converter << std::hex << hexStr;
    converter >> intValue;
    return static_cast<char>(intValue);
}


// Function to convert MIDI value to bipolar value
float midiToBipolar(int midiValue) {
    // Ensure the MIDI value is within the valid range (0 to 127)
    if (midiValue < 0) {
        midiValue = 0;
    } else if (midiValue > 127) {
        midiValue = 127;
    }

    // Calculate the bipolar value
    float bipolarValue = (midiValue - 64) / 64.0f * 64.0f;

    return bipolarValue;
}

// Function to open a SysEx file and return its content as a MIDI string
std::string fileToHexString(const std::string& filename)
{
    ifstream::pos_type size;
    uint8_t *memblock;

    ifstream file(filename, ios::in | ios::binary | ios::ate);
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return "";
    }

    size = file.tellg();
    memblock = new uint8_t[size];
    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char *>(memblock), size);
    file.close();

    // Convert binary data to a hexadecimal string
    std::ostringstream hexStream;

    for (int i = 0; i < size; ++i)
    {

        hexStream << hex << std::setw(2) << std::setfill('0') << static_cast<int>(memblock[i]);
//        cout << hex << std::setw(2) << std::setfill('0') << static_cast<int>(memblock[i]) << endl;

    }

//  Get the hexadecimal string and remove spaces
    std::string hexContent = hexStream.str();
//  hexContent.erase(std::remove(hexContent.begin(), hexContent.end(), ''), hexContent.end());

    delete[] memblock;


 // Troubleshooting Print File contents :
//    if (!hexContent.empty())
//    {
//        cout << "MIDI String:" << endl;
//        int maxColumn = 0;
//        for (size_t i = 0; i < hexContent.size(); ++i)
//        {
//            cout << hexContent[i];
//            maxColumn++;
//
//            if (maxColumn == 8)
//            {
//                cout << '\n';
//                maxColumn = 0;
//            }
//        }
//    }


    return hexContent;
}


std::string ConvertSoundProgramToXML(const std::string& midiString) {

    // Initialize variables to store the name portion and extract the name for the file
    std::vector<int> nameBytes;
    std::string name;


// Handle name bytes (last 16 bytes of body)
    for (int i = 496 ; i < 528; i += 2) {
        //       cout << midiString.substr(i, 2) << endl;
        std::string hexByte = midiString.substr(i, 2);
        name += hexStringToASCII(hexByte);

    }



    std::ofstream outputFile(name + ".xml"); // Replace with your output file name

    std::cout << name + ".xml created" << endl;


outputFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

//Name Patch

outputFile << "  <WWCFileSave version=\"1\" author=" << name << ">" << std::endl;
outputFile << "  <WWCTags Bright=\"0\" Dark=\"0\" Warm=\"0\" Cold=\"0\" Heavy=\"0\" Light=\"0\" Punchy=\"0\"\n"
              "           Mellow=\"0\" Rich=\"0\" Sparse=\"0\" Lush=\"0\" Layered=\"0\" Metallic=\"0\"\n"
              "           Wooden=\"0\" Gritty=\"0\" Smooth=\"0\" Detuned=\"0\" Plucky=\"0\" Short Decay=\"0\"\n"
              "           Long Release=\"0\" Echoey=\"0\" Reverb-heavy=\"0\" Distorted=\"0\" Clean=\"0\"\n"
              "           Wide=\"0\" Narrow=\"0\" Fat=\"0\" Thin=\"0\" Glitchy=\"0\" Analog=\"0\" Digital=\"0\"\n"
              "           Lo-fi=\"0\" Hi-fi=\"0\" Retro=\"0\" Modern=\"0\" Evolving=\"0\" Static=\"0\"\n"
              "           Aggressive=\"0\" Gentle=\"0\" Sustained=\"0\" Staccato=\"0\" Percussive=\"0\"\n"
              "           Harmonic=\"0\" Dissonant=\"0\" Soft Attack=\"0\" Hard Attack=\"0\" FM Synthesis=\"0\"\n"
              "           Granular=\"0\" Subtractive=\"0\" Additive=\"0\" Waveform=\"0\" Sequenced=\"0\"\n"
              "           Arpeggiated=\"0\" Modulated=\"0\" Mono=\"0\" Stereo=\"0\" Drone=\"0\" Noise-based=\"0\"\n"
              "           Ambient=\"0\" Synced=\"0\" Unsynced=\"0\" Velocity Sensitive=\"0\" Velocity Insensitive=\"0\"\n"
              "           Aftertouch Sensitive=\"0\" Polyphonic=\"0\" Monophonic=\"0\" Chorus=\"0\"\n"
              "           Flanged=\"0\" Phased=\"0\" Portamento=\"0\" Legato=\"0\" Stab=\"0\" Sweep=\"0\"\n"
              "           Resonant=\"0\" Deep=\"0\" High-pitched=\"0\" Low-pitched=\"0\" Midrange=\"0\"\n"
              "           Acid=\"0\" Ethereal=\"0\" Cinematic=\"0\" Industrial=\"0\" Experimental=\"0\"\n"
              "           Classic=\"0\" Iconic=\"0\" Textured=\"0\" Simple=\"0\" Complex=\"0\" Modwheel Modulated=\"0\"\n"
              "           Velocity Modulated=\"0\" Aftertouch Modulated=\"0\" Expressive=\"0\"\n"
              "           Vintage=\"0\" Futuristic=\"0\" Chiptune=\"0\" Robotic=\"0\" Organic=\"0\"\n"
              "           Crunchy=\"0\" Sweeping=\"0\" Hypnotic=\"0\"/>" << std::endl;
// Create the root elements

outputFile <<"<WWCAPVTS>" << std::endl;

//  Open the .syx file with OpenSysexFile and use the string it returns for the next parts:
//  if the file is a .syx and contains a valid header ( f03e 0300 0000 **** ) and is 532 characters it is a sound program and
//  send the hex string to a function called convertSoundProgramtoXML
//  move the first 16 characters to a new string called header
//  move the last 4 characters of the string to a new string called footer
//verify that the last two characters of the footer are f7
//return the body string after verifying it is 384 characters having removed the header and footer

    // Check if the MIDI string is 532  characters long
//    cout<<midiString.size()<<endl;

    if (midiString.size() == 532)  //0-531 = 532 right??? I thought it indexed from zero
    {
        // Extract the header (first 16 characters)
        std::string header = midiString.substr(0, 16);  // here i thought it would be 15 also. wrong
        cout << header << endl;

        // Extract the footer (last 4 characters)
        std::string footer = midiString.substr(midiString.size() - 4);

        cout << footer << endl;


        // Verify that the last two characters of the footer are "f7"
        ////figure out how to do the checksum if you want to get fancy
        if (footer.substr(2) != "f7") {
            cout << "sysex formatting error! no f7 close message" << endl;
            return 0;
        }
        else {

            // Extract the body (512 characters) after removing the header and footer
            std::string hexString = midiString.substr(17, 528);
//            cout << hexString << endl;




            // Process the cleaned hexString in pairs of two hex characters
            for (int i = 0; i < hexString.length(); i += 2) {
                std::string hexByte = hexString.substr(i, 2);



                // Convert the rest of the hex bytes to an integer
                if (i < 512) {
                    int intValue = hexStringToInt(hexByte);





                    //std::cout << hexByte[0] << hexByte[1] << " converted to " << intValue << std::endl; // For troubleshooting





                    std::string paramName;


                    // Define parameter names based on the provided index
                    //// added chopper spots but only as unused parameters that I take out later so the order is correct
                    switch (i) {
                        //i should put limits on illegal values like bend range
                        case 0: paramName = "osc1_octave"; break;
                        case 2: paramName = "osc1_semitone"; break;
                        case 4: paramName = "osc1_detune"; intValue = midiToBipolar(intValue); break;
                        case 6: paramName = "osc1_bendrange";  intValue = midiToBipolar(intValue); break;
                        case 8: paramName = "osc1_pitchmode"; break;
                        case 10: paramName = "osc1_mod1_source"; break;
                        case 12: paramName = "osc1_mod1_ctrl"; break;
                        case 14: paramName = "osc1_mod1_amt"; intValue = midiToBipolar(intValue);  break;
                        case 16: paramName = "osc1_mod2_source"; break;
                        case 18: paramName = "osc1_mod2_amt"; intValue = midiToBipolar(intValue);  break;
                        case 20: paramName = "osc1_mod2_quantize"; break;
                        case 22: paramName = "unused"; break;
                        case 24: paramName = "osc2_octave"; break;
                        case 26: paramName = "osc2_semitone"; break;
                        case 28: paramName = "osc2_detune"; intValue = midiToBipolar(intValue);  break;
                        case 30: paramName = "osc2_bendrange"; intValue = midiToBipolar(intValue);  break;
                        case 32: paramName = "osc2_pitchmode"; break;
                        case 34: paramName = "osc2_mod1_source"; break;
                        case 36: paramName = "osc2_mod1_ctrl"; break;
                        case 38: paramName = "osc2_mod1_amt"; intValue = midiToBipolar(intValue);  break;
                        case 40: paramName = "osc2_mod2_source"; break;
                        case 42: paramName = "osc2_mod2_amt"; intValue = midiToBipolar(intValue);  break;
                        case 44: paramName = "osc2_mod2_quantize"; break;
                        case 46: paramName = "osc2_link"; break;
                        case 48: paramName = "unused"; break;
                        case 50: paramName = "wave1_wavetable"; break;
                        case 52: paramName = "wave1_wavepos"; break;
                        case 54: paramName = "wave1_phase"; break;
                        case 56: paramName = "wave1_startmod_source"; break;
                        case 58: paramName = "wave1_startmod_amt"; intValue = midiToBipolar(intValue); break;
                        case 60: paramName = "wave1_env_amt"; intValue = midiToBipolar(intValue);  break;
                        case 62: paramName = "wave1_env_velamt";  intValue = midiToBipolar(intValue); break;
                        case 64: paramName = "wave1_kbcenter"; break;
                        case 66: paramName = "wave1_kbtrack";  intValue = midiToBipolar(intValue); break;
                        case 68: paramName = "wave1_mod1_source"; break;
                        case 70: paramName = "wave1_mod1_ctrl"; break;
                        case 72: paramName = "wave1_mod1_amt"; intValue = midiToBipolar(intValue);  break;
                        case 74: paramName = "wave1_mod2_source"; break;
                        case 76: paramName = "wave1_mod2_amt"; intValue = midiToBipolar(intValue);  break;
                        case 78: paramName = "wave1_mod2_quantize"; break;
                        case 80: paramName = "wave1_steppedsmooth"; break;
                        case 82: paramName = "unused"; break;
                        case 84: paramName = "wave2_startwave"; break;
                        case 86: paramName = "wave2_wavephase"; break;
                        case 88: paramName = "wave2_startmod_source"; break;
                        case 90: paramName = "wave2_startmod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 92: paramName = "wave2_env_amt";  intValue = midiToBipolar(intValue); break;
                        case 94: paramName = "wave2_env_velamt"; intValue = midiToBipolar(intValue);  break;
                        case 96: paramName = "wave2_kbtrack";  intValue = midiToBipolar(intValue); break;
                        case 98: paramName = "wave2_kbcenter"; break;
                        case 100: paramName = "wave2_mod1_source"; break;
                        case 102: paramName = "wave2_mod1_ctrl"; break;
                        case 104: paramName = "wave2_mod1_amt"; intValue = midiToBipolar(intValue);  break;
                        case 106: paramName = "wave2_mod2_source"; break;
                        case 108: paramName = "wave2_mod2_amt";  intValue = midiToBipolar(intValue); break;
                        case 110: paramName = "wave2_mod2_quantize"; break;
                        case 112: paramName = "wave2_steppedsmooth"; break;
                        case 114: paramName = "wave2_link"; break;
                        case 116: paramName = "unused"; break;
                        case 118: paramName = "wave1_vol"; break;
                        case 120: paramName = "wave2_vol"; break;
                        case 122: paramName = "noise_vol"; break;
                        case 124: paramName = "wave1_volmod_source"; break;
                        case 126: paramName = "wave1_volmod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 128: paramName = "wave2_volmod_source"; break;
                        case 130: paramName = "wave2_volmod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 132: paramName = "noise_volmod_source"; break;
                        case 134: paramName = "noise_volmod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 136: paramName = "amp_env_amt"; intValue = midiToBipolar(intValue);  break;
                        case 138: paramName = "amp_env_velamt";intValue = midiToBipolar(intValue);  break; break;
                        case 140: paramName = "amp_kbcenter"; break;
                        case 142: paramName = "amp_kbtrack"; break;
                        case 144: paramName = "amp_mod1_source"; break;
                        case 146: paramName = "amp_mod1_ctrl"; break;
                        case 148: paramName = "amp_mod1_amt"; break;
                        case 150: paramName = "amp_mod2_source"; break;
                        case 152: paramName = "amp_mod2_amt"; break;
                        case 154: paramName = "unused"; break;
                        case 156: paramName = "filter_mode"; break;
                        case 158: paramName = "filter_lp_freq"; break;
                        case 160: paramName = "filter_resonance"; break;
                        case 162: paramName = "filter_env_amt"; intValue = midiToBipolar(intValue);  break;
                        case 164: paramName = "filter_vel_amt";  intValue = midiToBipolar(intValue); break;
                        case 166: paramName = "filter_kbtrack"; intValue = midiToBipolar(intValue);  break;
                        case 168: paramName = "filter_kbcenter"; break;
                        case 170: paramName = "filter_mod1_source"; break;
                        case 172: paramName = "filter_mod1_ctrl"; break;
                        case 174: paramName = "filter_mod1_amt";  intValue = midiToBipolar(intValue); break;
                        case 176: paramName = "filter_mod2_source"; break;
                        case 178: paramName = "filter_mod2_amt";  intValue = midiToBipolar(intValue); break;
                        case 180: paramName = "filter_resmod_source"; break;
                        case 182: paramName = "filter_resmod_ctrl"; break;
                        case 184: paramName = "filter_resmod_amt";  intValue = midiToBipolar(intValue); break;
                        case 186: paramName = "dualmode_hp_freq"; break;
                        case 188: paramName = "dualmode_hp_env_select"; break;
                        case 190: paramName = "dualmode_hp_env_amt";intValue = midiToBipolar(intValue);   break;
                        case 192: paramName = "dualmode_hp_vel_amt"; intValue = midiToBipolar(intValue);  break;
                        case 194: paramName = "dualmode_hp_keytrack"; break;
                        case 196: paramName = "dualmode_hp_keycenter"; break;
                        case 198: paramName = "dualmode_hp_mod1_source"; break;
                        case 200: paramName = "dualmode_hp_mod1_ctrl"; break;
                        case 202: paramName = "dualmode_hp_mod1_amt";intValue = midiToBipolar(intValue);  break;
                        case 204: paramName = "dualmode_hp_mod2_source"; break;
                        case 206: paramName = "dualmode_hp_mod2_amt"; break;
                        case 208: paramName = "bandpass_filter_bandwidth"; break;
                        case 210: paramName = "unused"; break;
                        case 212: paramName = "amp_env__atk"; break;
                        case 214: paramName = "amp_env_decay"; break;
                        case 216: paramName = "amp_env_sustain"; break;
                        case 218: paramName = "amp_env_release"; break;
                        case 220: paramName = "amp_env__atk_mod_source"; break;
                        case 222: paramName = "amp_env__atk_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 224: paramName = "amp_env_decay_mod_source"; break;
                        case 226: paramName = "amp_env_decay_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 228: paramName = "amp_env_sustain_mod_source"; break;
                        case 230: paramName = "amp_env_sustain_mod_amt";  intValue = midiToBipolar(intValue); break;
                        case 232: paramName = "amp_env_release_mod_source"; break;
                        case 234: paramName = "amp_env_release_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 236: paramName = "unused"; break;
                        case 238: paramName = "filter_env_delay"; break;
                        case 240: paramName = "filter_env_atk"; break;
                        case 242: paramName = "filter_env_decay"; break;
                        case 244: paramName = "filter_env_sustain"; break;
                        case 246: paramName = "filter_env_release"; break;
                        case 248: paramName = "unused"; break;
                        case 250: paramName = "unused"; break;
                        case 252: paramName = "filter_env_atk_mod_source"; break;
                        case 254: paramName = "filter_env_atk_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 256: paramName = "filter_env_dec_mod_source"; break;
                        case 258: paramName = "filter_env_dec_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 260: paramName = "filter_env_sustain_mod_source"; break;
                        case 262: paramName = "filter_env_sustain_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 264: paramName = "filter_env_release_mod_source"; break;
                        case 266: paramName = "filter_env_release_mod_amt";  intValue = midiToBipolar(intValue); break;
                        case 268: paramName = "unused"; break;
                        case 270: paramName = "wave_env_time_1"; break;
                        case 272: paramName = "wave_env_level_1"; break;
                        case 274: paramName = "wave_env_time_2"; break;
                        case 276: paramName = "wave_env_level_2"; break;
                        case 278: paramName = "wave_env_time_3"; break;
                        case 280: paramName = "wave_env_level_3"; break;
                        case 282: paramName = "wave_env_time_4"; break;
                        case 284: paramName = "wave_env_level_4"; break;
                        case 286: paramName = "wave_env_time_5"; break;
                        case 288: paramName = "wave_env_level_5"; break;
                        case 290: paramName = "wave_env_time_6"; break;
                        case 292: paramName = "wave_env_level_6"; break;
                        case 294: paramName = "wave_env_time_7"; break;
                        case 296: paramName = "wave_env_level_7"; break;
                        case 298: paramName = "wave_env_time_8"; break;
                        case 300: paramName = "wave_env_level_8"; break;
                        case 302: paramName = "wave_env_timemod_source"; break;
                        case 304: paramName = "wave_env_timemod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 306: paramName = "wave_env_levelmod_source"; break;
                        case 308: paramName = "wave_env_levelmod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 310: paramName = "wave_env_key_off_point"; break;
                        case 312: paramName = "wave_env_loop_start_point"; break;
                        case 314: paramName = "wave_env_loop_onoff"; break;
                        case 316: paramName = "unused"; break;
                        case 318: paramName = "free_env_time_1"; break;
                        case 320: paramName = "free_env_level_1"; break;
                        case 322: paramName = "free_env_time_2"; break;
                        case 324: paramName = "free_env_level_2"; break;
                        case 326: paramName = "free_env_time_3"; break;
                        case 328: paramName = "free_env_level_3"; break;
                        case 330: paramName = "free_env_time_4"; break;
                        case 332: paramName = "free_env_level_4"; break;
                        case 334: paramName = "free_env_time_mod_source"; break;
                        case 336: paramName = "free_env_time_mod_amt";  intValue = midiToBipolar(intValue); break;
                        case 338: paramName = "free_env_level_mod_source"; break;
                        case 340: paramName = "free_env_level_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 342: paramName = "free_env_zero_axis"; break;
                        case 344: paramName = "lfo1_rate"; break;
                        case 346: paramName = "lfo1_shape"; break;
                        case 348: paramName = "lfo1_symmetry"; break;
                        case 350: paramName = "lfo1_humanize"; break;
                        case 352: paramName = "lfo1_rate_mod_source"; break;
                        case 354: paramName = "lfo1_rate_mod_amt"; break;
                        case 356: paramName = "lfo1_level_mod_source"; break;
                        case 358: paramName = "lfo1_level_mod_ctrl"; break;
                        case 360: paramName = "lfo1_level_mod_amt"; break;
                        case 362: paramName = "lfo1_sync"; break;
                        case 364: paramName = "lfo2_rate"; break;
                        case 366: paramName = "lfo2_shape"; break;
                        case 368: paramName = "lfo2_symmetry"; break;
                        case 370: paramName = "lfo2_humanize"; break;
                        case 372: paramName = "lfo2_rate_mod"; break;
                        case 374: paramName = "lfo2_rate_mod"; break;
                        case 376: paramName = "lfo2_levelmod_source"; break;
                        case 378: paramName = "lfo2_levelmod_ctrl"; break;
                        case 380: paramName = "lfo2_levelmod_amt"; break;
                        case 382: paramName = "lfo2_sync"; break;
                        case 384: paramName = "ctrl_ramp_trigger_source"; break;
                        case 386: paramName = "ctrl_ramp_rate"; intValue = midiToBipolar(intValue);  break;
                        case 388: paramName = "panning_source1"; break;
                        case 390: paramName = "panning_ctrl1"; break;
                        case 392: paramName = "panning_amt1"; intValue = midiToBipolar(intValue);  break;
                        case 394: paramName = "panning_source2"; break;
                        case 396: paramName = "panning_amt2"; intValue = midiToBipolar(intValue); break;
                        case 398: paramName = "controlcomp_source"; break;
                        case 400: paramName = "controlcomp_threshold"; intValue = midiToBipolar(intValue);  break;
                        case 402: paramName = "controlmix1_source"; break;
                        case 404: paramName = "controlmix1_amt"; intValue = midiToBipolar(intValue);  break;
                        case 406: paramName = "controlmix2_source"; break;
                        case 408: paramName = "controlmix2_amt";  intValue = midiToBipolar(intValue); break;
                        case 410: paramName = "controlmix3_source"; break;
                        case 412: paramName = "controlmix3_amt";  intValue = midiToBipolar(intValue); break;
                        case 414: paramName = "controldelay_source"; break;
                        case 416: paramName = "controldelay_time"; break;
                        case 418: paramName = "controldelay_timemod_source"; break;
                        case 420: paramName = "controldelay_timemod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 422: paramName = "controlshaper_source"; break;
                        case 424: paramName = "controlshaper_ref_1"; break;
                        case 426: paramName = "controlshaper_ref_2"; break;
                        case 428: paramName = "controlshaper_ref_3"; break;
                        case 430: paramName = "controlshaper_ref_4"; break;
                        case 432: paramName = "controlshaper_ref_5"; break;
                        case 434: paramName = "controlshaper_ref_6"; break;
                        case 436: paramName = "controlshaper_ref_7"; break;
                        case 438: paramName = "controlshaper_ref_8"; break;
                        case 440: paramName = "controlshaper_ref_9"; break;
                        case 442: paramName = "sh_source"; break;
                        case 444: paramName = "sh_rate"; break;
                        case 446: paramName = "sh_rate_mod_source"; break;
                        case 448: paramName = "sh_rate_mod_amt"; intValue = midiToBipolar(intValue);  break;
                        case 450: paramName = "unused"; break;
                        case 452: paramName = "aux_level_mod_source"; break;
                        case 454: paramName = "aux_level_mod_control"; break;
                        case 456: paramName = "aux_level_mod_amt"; intValue = midiToBipolar(intValue) ; break;
                        case 458: paramName = "aux_level_min"; break;
                        case 460: paramName = "unused"; break;
                        case 462: paramName = "unused"; break;
                        case 464: paramName = "unused"; break;
                        case 466: paramName = "glide_mode"; break;
                        case 468: paramName = "glide_rate"; break;
                        case 470: paramName = "glide_slope"; break;
                        case 472: paramName = "glide_time_mod_source"; break;
                        case 474: paramName = "glide_time_mod_amt"; break;
                        case 476: paramName = "glide_onoff"; break;
                        case 478: paramName = "valid_flag"; break;
                        case 480: paramName = "name0" ; break;
                        case 482: paramName = "name1" ; break;
                        case 484: paramName = "name2" ; break;
                        case 486: paramName = "name3" ; break;
                        case 488: paramName = "name4" ; break;
                        case 490: paramName = "name5" ; break;
                        case 492: paramName = "name6" ; break;
                        case 494: paramName = "name7" ; break;
                        case 496: paramName = "name8" ; break;
                        case 498: paramName = "name9" ; break;
                        case 500: paramName = "namea" ; break;
                        case 502: paramName = "nameb" ; break;
                        case 504: paramName = "namec" ; break;
                        case 506: paramName = "named" ; break;
                        case 508: paramName = "namee" ; break;
                        case 510: paramName = "namef" ; break;
                        case 512: paramName = "nameg" ; break;

                        default: paramName = "Parameter" + std::to_string(i / 2); break;
                    }

                    if (paramName != "unused" && paramName != "valid_flag")  {

                        double paramValue = static_cast<double>(intValue);

                        // Write the parameter to the XML file
                        outputFile << "  <PARAM id=\"" << paramName << "\" value=\"" << paramValue << "\"/>"
                                   << std::endl;
                    }
                }

            }

            //write the unassigned parameters that the patch data does not contain
                    outputFile <<"  <PARAM id=\"bank\" value=\"1.0\"/>" << std::endl;
                    outputFile <<"  <PARAM id=\"change\" value=\"1\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"midichannel\" value=\"1.0\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_x\" value=\"46.0\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_y\" value=\"47.0\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"volchange\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_x_1\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_x_2\" value=\"46.0\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_x_3\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_x_val\" value=\"1.0\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_y_1\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_y_2\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_y_3\"/>"<< std::endl;
                    outputFile <<"  <PARAM id=\"xy_y_val\" value=\"1.0\"/>"<< std::endl;



            // Close the root element
            outputFile << "</WWCAPVTS>" << std::endl;

            // Close the root element
            outputFile << "</WWCFileSave>" << std::endl;

            // Close the input and output files

            outputFile.close();

            std::cout << "Conversion complete!" << std::endl;

        }
    }

    // Return an empty string if the MIDI program is not valid
    return "";
}

// Function to remove the initial bulk switch message from a MIDI string
//std::string RemoveBulkSwitch(const std::string& midiString)
//{
//    std::string result = midiString;
//
//    // Define the initial bulk switch message
//    const std::string bulkSwitchMessage = "f03e03000f0101f7";
//
//    // Check if the MIDI string starts with the bulk switch message
//    if (result.compare(0, bulkSwitchMessage.size(), bulkSwitchMessage) == 0)
//    {
//        // Remove the bulk switch message from the beginning
//        result.erase(0, bulkSwitchMessage.size());
//    }
//
//    return result;
//}



// Function to process MIDI string in 532-character chunks
void BankStringToSoundProgram(const std::string& midiString)
{
    std::string remainingMIDI =midiString;

    while (!remainingMIDI.empty()) {
        if (remainingMIDI.size() > 532) {
            // Extract a 532-character chunk from the remaining MIDI string
            std::string chunk = remainingMIDI.substr(0, 532);

            // Process the chunk using ConvertSoundProgramToXML
            ConvertSoundProgramToXML(chunk);

            // Remove the processed chunk from the remaining MIDI string
            remainingMIDI.erase(0, chunk.size());
        }
        else {
            cout << "Bank Footer Message : " << remainingMIDI << endl;
            remainingMIDI.erase();
        }
    }
}




int main()
{
    std::string parameter = "DavidSoundBankA.syx";

    // Call the function to open the SysEx file and return the MIDI string
    std::string midiString = fileToHexString(parameter);


    if (midiString.length() == 532) {
        ConvertSoundProgramToXML(midiString);

    }
    else if ( midiString.length() > 532 ) {

        BankStringToSoundProgram(midiString);

    }

    //  later to open internet sound banks we have : determine if the input file has one of these extensions : soundfile (*.snd), bankfile (*.set), performancefile (.tbd), arrangementfile (*.arr) those each need their own conditions to be determined later




    //  if the file is .syx and contains a header ( f03e 0300 000f **** ) or (f03e 0300 0f01 01f7)  and is longer than 532 characters , it is a sound bank sysex dump
        //send the hex string to a function called convertBanktoSoundPrograms
            //there we take 532 character chunks from the string and send them to convertSoundProgramtoXML



    return 0;
}
