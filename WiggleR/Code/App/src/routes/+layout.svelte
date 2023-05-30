<script lang="ts">
	import { start, end, showVideo, showContours } from '../store/store';
	import { goto } from '$app/navigation';
	import { onMount } from 'svelte';
	import ImageSelector from '../lib/ImageSelector.svelte';

	let openImageSelector: Boolean | 'start' | 'end' = false;
	let startTime = '';
	let endTime = '';
	let videoActive: boolean = false;
	let contoursActive: boolean = false;

	function close() {
		openImageSelector = false;
	}

	function scheduleStart() {
		openImageSelector = 'start';
	}
	function scheduleEnd() {
		openImageSelector = 'end';
	}

	function onChangeVideo(e: { target: { active: boolean } }) {
		showVideo.update((show) => (show = e.target.active));
	}
	function onChangeContours(e: { target: { active: boolean } }) {
		showContours.update((show) => (show = e.target.active));
	}

	function imageSelected(e: CustomEvent) {
		if (openImageSelector === 'start') {
			start.update((start) => (start = e.detail));
		}
		if (openImageSelector === 'end') {
			end.update((end) => (end = e.detail));
		}
		openImageSelector = false;
	}

	onMount(async () => {
		await import('@kor-ui/kor');
		await import('@kor-ui/kor/kor-styles.css');
	});

	start.subscribe((start) => (startTime = start));
	end.subscribe((end) => (endTime = end));
	showVideo.subscribe((show) => (videoActive = show));
	showContours.subscribe((show) => (contoursActive = show));
</script>

<kor-page>
	<kor-app-bar slot="top" label="WiggleR">
		<kor-tabs>
			<kor-tab-item on:click={() => goto('/timelapse')} label="Timelapse" />
			<kor-tab-item on:click={() => goto('/timeline')} label="Timeline" />
			<kor-tab-item on:click={() => goto('/test')} label="Test" />
		</kor-tabs>

		<div class="actionsBar">
			<kor-button icon="schedule" label={startTime || 'Start'} on:click={scheduleStart} />
			<kor-button icon="schedule" label={endTime || 'End'} on:click={scheduleEnd} />
			<kor-toggle label="Video" on:active-changed={onChangeVideo} active={videoActive ? true : null}></kor-toggle>
			<kor-toggle label="Contours" on:active-changed={onChangeContours}  active={contoursActive ? true : null}></kor-toggle>
		</div>
		{#if openImageSelector}
			<ImageSelector on:select={imageSelected} on:close={close} />
		{/if}
	</kor-app-bar>

	<slot />
</kor-page>

<style>
	.actionsBar {
		display: flex;
		gap: 10px;
	}
</style>
