# Group-15 Shopping Assistant

## **What are we going to make?**

- Our group has developed a shopping assistant which can be used in various types of stores, offices and in the health care sector. The project is based on a remote-controlled car with a display which customers, guests or patients interact with.

## **Why will we make it?**

- The need for non-personal assistance in different sectors is growing every year and unloading simplistic tasks on software is a cost-efficient way to solve this issue. Our vision is that this assistant will be able to help companies in different sectors to minimize the need for personnel to dedicate time to complete simplistic tasks which can now be done by machines. The assistant will also enable customers to easily get information and thereby save time when visiting a grocery store for example.

## **What problem does it solve?**

- Overcome the problem of navigation in large stores.

## **How are we going to make it?**

- We have done this by creating a website which will be running on the display of the shopping assistant while it navigates through a premise. The users can interact with the display and with speech functionality the user can ask the assistant questions and be given a voiced or visual response based on the question. The need to cover a large area and not be stationary enables the assistant to interact with more users. Since we want the need of personal and shopping assistant interactions to be minimized, the assistant is equipped with different kinds of self-navigation modes.

## **Link to the demo video**

- https://www.youtube.com/watch?v=VPUr9Y1GWiA

## **What kind of technology have we used?**

    C++, HTML, CSS
    Web server
    Voice recognition
    Arduino
    Mqtt broker

## **System diagram**

![Assistant Diagram](https://user-images.githubusercontent.com/90680904/170860029-c5408425-946f-41ca-8c2a-7f29d9ea52e5.png)

### **Diagram Discription**

The system consists of three parts, the ardurino car, mqtt broker and the website. The shopping assistant and the website communicate via the mqtt broker. This enables the the user to controll the car from the website by publishing topics to the broker.

## **Getting started**

**1.** Clone the repository.
Please observe that step 1.0-1.7 is instruction for Mac. Please use the right steps for your operating system.

**1.1** Go to the main page of the repository.

**1.2** Above the list of files, click Code.

**1.3** Copy the URL for the repository.

**1.4** Open Terminal.

**1.5** Change the current working directory to the location where you want the cloned directory.

**1.6** Type git clone, and then paste the URL you copied earlier.
`git clone https://github.com/YOUR-USERNAME/YOUR-REPOSITORY`

**1.7** Press Enter to create your local clone.

**2.** Open the project in an IDE of your choice.

**3.** When project is loaded, open the terminal in the IDE
**3.1** Install all dependencies, write this in the IDE terminal: npm install
**3.2** To start the server write this in the IDE terminal: npm start

**4.** Open google browser and enter: `http://localhost:3000`

**5.** If the physical car is not available you can install this emulator for the car. Follow the instructions in the link below to install the emulator.

**Mac:** <https://github.com/ItJustWorksTM/smce-gd/wiki/MacOS-setup>

**Windows:** <https://github.com/ItJustWorksTM/smce-gd/wiki/Windows-setup>
