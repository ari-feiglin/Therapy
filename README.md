# **AUTOMATED THERAPY**  

Using this program, therapizing yourself is a breeze!  

***
### **USAGE**
Like all C programs, clone or pull the repository from github and then run make. This should compile into a binary called "therapist", run it.  
The program should greet you with a magenta line over >>. When you add responses to Therapy Bot, that magenta line will fill up.  
You can write anything you want into the program, but Therapy Bot may not recognize it.  
If you want to add a response to Therapy Bot, simply write "!add" (no quotation marks), and follow the following steps:
1. When asked "User prompt" type in something you would like to say to Therapy Bot.
2. When asked "Therapist response" type in Therapy Bot's response
3. When asked "Number of responses" type in the number of responses the user can respond to Therapy Bot's response with. If this is not 0, it will continue to ask this.  

Here's an example of me adding the word "Hello":
```
>> !add
User prompt: Hello
Therapist response: Howdy! How are you?
Number of responses: 2
User prompt: I'm good
Therapist response: That's good to hear. Have you done anything interesting?
Number of responses: 2
User prompt: Yes
Therapist response: Nice, it's good to keep your mind active.
Number of responses: 0
User prompt: No
Therapist response: Oh, you should try. It's good to keep your mind active.
Number of responses: 0
User prompt: I'm not feeling so great.
Therapist response: Oh shame, how come?
Number of responses: 2
User prompt: Life's got me down.
Therapist response: Yeah, it'll do that.
Number of responses: 0
User prompt: I don't know, I just feel bummed
Therapist response: Yeah, life can do that to you.
Number of responses: 0
```

So now, if I write "Hello", Therapy Bot's response will be "Howdy! How are you?", I can then respond with either "I'm good" or "I'm not feeling so great". Therapy Bot responds to "I'm good" by saying "That's good to hear. Have you done anything interesting?", which you can respond to by saying "Yes" or "No", and so on...

I'll try and split that segment up:
```
>> !add
User prompt: Hello
Therapist response: Howdy! How are you?
Number of responses: 2
---------------------------------------------------------------------------
| User prompt: I'm good
| Therapist response: That's good to hear. Have you done anything interesting?
| Number of responses: 2
---------------------------------------------------------------------------
|     User prompt: Yes
|     Therapist response: Nice, it's good to keep your mind active.
|     Number of responses: 0
---------------------------------------------------------------------------
|     User prompt: No
|     Therapist response: Oh, you should try. It's good to keep your mind
|     active.
|     Number of responses: 0
---------------------------------------------------------------------------
|  User prompt: I'm not feeling so great.
|  Therapist response: Oh shame, how come?
|  Number of responses: 2
---------------------------------------------------------------------------
|     User prompt: Life's got me down.
|     Therapist response: Yeah, it'll do that.
|     Number of responses: 0
---------------------------------------------------------------------------
|     User prompt: I don't know, I just feel bummed
|     Therapist response: Yeah, life can do that to you.
|     Number of responses: 0
---------------------------------------------------------------------------
```

I'll probably make this more intuitive later.  
You can only use !add when not already in a conversation with the Therapy Bot. Writing a message the Therapy Bot doesn't understand will quit your current conversation, going back to the "main menu", where you can call !add.  
To be honest, nothing is forcing you to use this as a therapist. You can make your own (pretty terrible) chatbot with this, too.  

I added an example file (therapy.tb) that has some premade responses, it's not too much, it's just to give you a place to start.

<u>**This program, in no way, shape, or form, is meant to be anything other than a joke. Do not use this program instead of getting actual mental help from a professional.**</u>
